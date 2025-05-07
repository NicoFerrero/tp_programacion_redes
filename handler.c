#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

void *handleClient(void *arg)
{
    int clientSocket = *(int *)arg;
    char buffer[1024];
    ssize_t bytesRead;
    free(arg); /*Liberamos la memoria asignada dinámicamente en server.c*/
    memset(buffer, 0, 1024);

    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        printf("Mensaje recibido: %s\n", buffer);

        if (strcmp(buffer, "PING") == 0)
        {
            send(clientSocket, "PONG", 4, 0);
        }
    }

    close(clientSocket);
    return NULL;
}
