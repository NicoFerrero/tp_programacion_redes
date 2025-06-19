#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 8080

/*int main(int argc, char *argv[])
{
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    int sock;
    struct sockaddr_in serverAddr = {0};
    char message[] = "PING";
    char buffer[1024];
    ssize_t bytesReceived;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error en connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    send(sock, message, strlen(message), 0);

    bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    close(sock);
    return 0;
}*/

int main(int argc, char *argv[])
{
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    int sock;
    struct sockaddr_in serverAddr = {0};
    char buffer[2048];
    ssize_t bytesReceived;

    const char *httpRequest = "GET /ping HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error en connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    send(sock, httpRequest, strlen(httpRequest), 0);

    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytesReceived] = '\0';
        printf("%s", buffer);
    }

    printf("\n");
    close(sock);
    return 0;
}