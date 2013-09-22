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

#include "gvars.h"
#include "init/init.h"
#include "log/log.h"
#include "listener/listener.h"

static unsigned int test_flags;
static char volatile abort_loop = 0;
static unsigned int select_errors;

static void handle_connections_sockets()
{
    int sock = 0, new_sock = 0, i = 0;
    uint error_count = 0, retry = 0;
    struct sockaddr_storage cAddr;
    int ip_flags = 0, flags = 0, retval;
    int socket_count = 0;
    struct pollfd fds[2]; // for ip_sock and unix_sock

    if (ip_sock != -1)
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
            if (new_sock != -1 ||
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

        if (new_sock == -1)
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
        printf("process\n");
    }
    return;
}

int tc(int argc, char** argv)
{
    char *dir = "/export/servers/data/mysql/dba/manager/dbfile/ng.log";
    unsigned int port = 1111, mport = 2222;

    first_init();

    set_port(&port);
    set_mport(&mport);
    set_servername(NULL);
    set_version();
    set_basedir(dir);
    set_datadir(dir);
    set_backupdir(dir);

    printf("variables initialization\n");
    printf("servername=%s\n", server_name);
    printf("version=%s\n", server_version);
    printf("tc_port=%u\n", tc_port);
    printf("mport=%u\n", manage_port);
    printf("basedir=%s\n", basedir);
    printf("datadir=%s\n", datadir);
    printf("backupdir=%s\n\n", backupdir);
        
    listener_init();
    
    handle_connections_sockets();
    
    return 0;
}