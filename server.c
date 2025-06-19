#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <semaphore.h>
#include "server.h"
#include "handler.h"
#include "threadpool.h"

int setup_server_socket(int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    struct sockaddr_in serverAddr = {0};

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 10);

    return sockfd;
}

void run_thread_mode(int serverSocket)
{
    while (1)
    {
        struct sockaddr_in client;
        pthread_t tid;
        socklen_t len = sizeof(client);
        int *arg = malloc(sizeof(int));

        int clientSocket = accept(serverSocket, (struct sockaddr *)&client, &len);

        if (clientSocket < 0)
        {
            perror("accept");
            continue;
        }

        *arg = clientSocket;
        pthread_create(&tid, NULL, handle_client_threadmode, arg);
        pthread_detach(tid);
    }
}

void run_select_mode(int serverSocket)
{
    int fdmax = serverSocket;
    fd_set master, readfds;
    int i;

    FD_ZERO(&master);
    FD_SET(serverSocket, &master);

    while (1)
    {
        readfds = master;
        if (select(fdmax + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("select");
            break;
        }

        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (i == serverSocket)
                {
                    int newfd = accept(serverSocket, NULL, NULL);
                    FD_SET(newfd, &master);
                    if (newfd > fdmax)
                        fdmax = newfd;
                }
                else
                {
                    handle_client(i);
                    FD_CLR(i, &master);
                }
            }
        }
    }
}

void task_runnable(void *param)
{
    int clientSocket = *((int *)param);
    handle_client(clientSocket);
    free(param);
}

void run_pool_mode(int serverSocket)
{
    threadpool_t *pool = threadpool_create(4, 8, 0);

    while (1)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        int *arg = malloc(sizeof(int));
        *arg = clientSocket;

        threadpool_add(pool, task_runnable, arg, 0);
    }
}
