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

#include "../tc_global.h"

static unsigned short tc_port_timeout = 10;
static unsigned int back_log = 1024;
static char *my_bind_addr_str = NULL;

void listener_init()
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
            if (ip_sock != -1)
                break;
        }

        if (ip_sock == -1)
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
