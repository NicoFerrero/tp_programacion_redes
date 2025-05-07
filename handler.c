#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

void *handleClient(void *arg)
{
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Servidor C funcionando</h1>";
    const char *notFound =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Imagen no encontrada";
    char header[256];
    char buffer[4096];
    ssize_t bytesRead;
    char verb[8], path[1024], version[16];
    long imgSize;
    FILE *img;
    char *imgData;

    int clientSocket = *(int *)arg;
    free(arg);

    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0)
    {
        close(clientSocket);
        return NULL;
    }

    buffer[bytesRead] = '\0';

    sscanf(buffer, "%s %s %s", verb, path, version);

    printf("HTTP Request\n");
    printf("Verbo: %s\n", verb);
    printf("Ruta: %s\n", path);
    printf("Versión: %s\n", version);

    if (strcmp(path, "/conejo") == 0)
    {
        img = fopen("conejo.jpg", "rb");
        if (!img)
        {
            send(clientSocket, notFound, strlen(notFound), 0);
            close(clientSocket);
            return NULL;
        }

        fseek(img, 0, SEEK_END);
        imgSize = ftell(img);
        rewind(img);

        imgData = malloc(imgSize);
        fread(imgData, 1, imgSize, img);
        fclose(img);

        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: image/jpeg\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n"
                 "\r\n",
                 imgSize);

        send(clientSocket, header, strlen(header), 0);
        send(clientSocket, imgData, imgSize, 0);

        free(imgData);
    }
    else
    {
        send(clientSocket, response, strlen(response), 0);
    }

    close(clientSocket);
    return NULL;
}
