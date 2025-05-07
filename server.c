#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "handler.h"

#define DEFAULT_PORT 8080

int main(int argc, char *argv[])
{
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    struct sockaddr_in serverAddr = {0};
    pthread_t thread;
    int serverSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientLen;
    int *clientSocket;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error en bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) < 0)
    {
        perror("Error en listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", port);

    while (1)
    {
        clientLen = sizeof(clientAddr);
        clientSocket = malloc(sizeof(int));

        *clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
        if (*clientSocket < 0)
        {
            perror("Error en accept");
            free(clientSocket);
            continue;
        }

        if (pthread_create(&thread, NULL, handleClient, clientSocket) != 0)
        {
            perror("Error al crear el thread");
            free(clientSocket);
        }
        else
        {
            pthread_detach(thread);
        }
    }

    close(serverSocket);
    return 0;
}
