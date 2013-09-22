#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include "../gvars.h"

#define INVALID_SOCKET  -1

static char volatile abort_loop = 0;
static unsigned int select_errors;
static unsigned int test_flags;
static unsigned short tc_port_timeout = 10;
static unsigned int back_log = 1024;
static char *my_bind_addr_str = NULL;
static int ip_sock;

void network_init()
{
    int ret, arg;
    uint waited;
    uint this_wait;
    uint retry;
    char port_buf[NI_MAXSERV];

    ret = 0;

    if (tc_port != 0)
    {
        struct addrinfo *ai, *a;
        struct addrinfo hints;
        int error;
        printf("IP Socket is %d\n", tc_port);

        bzero(&hints, sizeof (hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_UNSPEC;

        snprintf(port_buf, NI_MAXSERV, "%d", tc_port);
        error = getaddrinfo(my_bind_addr_str, port_buf, &hints, &ai);
        printf("ai->ai_family=%d, ai->ai_protocol=%d\n", ai->ai_family, ai->ai_protocol);
        if (error != 0)
        {
            printf("error Got error: %d from getaddrinfo()\n", error);
            perror((char *) (ELIBSCN)); /* purecov: tested */
            exit(1); /* purecov: tested */
        }

        for (a = ai; a != NULL; a = a->ai_next)
        {
            ip_sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
            if (ip_sock != INVALID_SOCKET)
                break;
        }

        if (ip_sock == INVALID_SOCKET)
        {
            printf("error Got error: %d from socket()\n", errno);
            perror((char *) (ELIBSCN)); /* purecov: tested */
            exit(1); /* purecov: tested */
        }

        arg = 1;
        (void) setsockopt(ip_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &arg, sizeof (arg));

        /*
          Sometimes the port is not released fast enough when stopping and
          restarting the server. This happens quite often with the test suite
          on busy Linux systems. Retry to bind the address at these intervals:
          Sleep intervals: 1, 2, 4,  6,  9, 13, 17, 22, ...
          Retry at second: 1, 3, 7, 13, 22, 35, 52, 74, ...
          Limit the sequence by tc_port_timeout (set --port-open-timeout=#).
         */
        for (waited = 0, retry = 1;; retry++, waited += this_wait)
        {
            if (((ret = bind(ip_sock, a->ai_addr, a->ai_addrlen)) >= 0) ||
                    (errno != EADDRINUSE) ||
                    (waited >= tc_port_timeout))
                break;
            printf("Retrying bind on TCP/IP port %u\n", tc_port);
            this_wait = retry * retry / 3 + 1;
            sleep(this_wait);
        }
        freeaddrinfo(ai);
        if (ret < 0)
        {
            printf("error Got error: %d from bind\n", errno);
            perror("Can't start server: Bind on TCP/IP port\n");
            printf("Do you already have another mysqld server running on port: %d ?\n", tc_port);
            exit(1);
        }
        if (listen(ip_sock, (int) back_log) < 0)
        {
            perror("Can't start server: listen() on TCP/IP port\n");
            printf("listen() on TCP/IP failed with error %d\n",
                    errno);
            exit(1);
        }
    } else
        exit(1);

    printf("server started\n");
    return;
}

void handle_connections_sockets()
{
    int sock = 0, new_sock = 0, i = 0;
    uint error_count = 0, retry = 0;
    struct sockaddr_storage cAddr;
    int ip_flags = 0, flags = 0, retval;
    int socket_count = 0;
    struct pollfd fds[2]; // for ip_sock and unix_sock

    if (ip_sock != INVALID_SOCKET)
    {
        fds[socket_count].fd = ip_sock;
        fds[socket_count].events = POLLIN;
        socket_count++;
        ip_flags = fcntl(ip_sock, F_GETFL, 0);
    }

    printf("Waiting for connections.\n");
    while (!abort_loop)
    {
        retval = poll(fds, socket_count, -1);

        if (retval < 0)
        {
            if (errno != EINTR)
            {
                if (!select_errors++ && !abort_loop) /* purecov: inspected */
                    printf("Got error %d from select\n", errno); /* purecov: inspected */
            }
            continue;
        }

        /* Is this a new connection request ? */
        for (i = 0; i < socket_count; ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                sock = fds[i].fd;
                flags = fcntl(sock, F_GETFL, 0);
                break;
            }
        }

        if (!(test_flags & 8))
        {
            fcntl(sock, F_SETFL, flags | O_NONBLOCK);
        }

        for (retry = 0; retry < 10; retry++)
        {
            socklen_t length = sizeof (struct sockaddr_storage);
            new_sock = accept(sock, (struct sockaddr *) (&cAddr),
                    &length);
            if (new_sock != INVALID_SOCKET ||
                    (errno != EINTR && errno != EAGAIN))
                break;
            if (!(test_flags & 8))
            {
                if (retry == 10 - 1)
                    fcntl(sock, F_SETFL, flags); // Try without O_NONBLOCK
            }
        }

        if (!(test_flags & 8))
            fcntl(sock, F_SETFL, flags);

        if (new_sock == INVALID_SOCKET)
        {
            if ((error_count++ & 255) == 0) // This can happen often
                printf("Error in accept\n");
            if (errno == ENFILE || errno == EMFILE)
                sleep(1); // Give other threads some time
            continue;
        }

        socklen_t dummyLen;
        struct sockaddr_storage dummy;
        dummyLen = sizeof (dummy);
        if (getsockname(new_sock, (struct sockaddr *) &dummy,
                (socklen_t *) & dummyLen) < 0)
        {
            printf("Error on new connection socket\n");
            (void) shutdown(new_sock, SHUT_RDWR);
            (void) close(new_sock);
            continue;
        }

        //处理请求
    }
    return;
}
