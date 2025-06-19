#include "handler.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 2048
#define RESPONSE_SIZE 1024

int ends_with(const char *str, const char *suffix)
{
    size_t lenstr, lensuffix;
    if (!str || !suffix)
        return 0;
    lenstr = strlen(str);
    lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void send_image(int clientSocket, const char *path)
{
    struct stat st;
    char header[RESPONSE_SIZE];
    int imagefd = open(path, O_RDONLY);
    off_t offset = 0;
    ssize_t sent;

    if (imagefd < 0)
    {
        perror("open image");
        return;
    }

    fstat(imagefd, &st);

    snprintf(header, RESPONSE_SIZE,
             "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: %ld\r\nConnection: Close\r\n\r\n",
             st.st_size);
    send(clientSocket, header, strlen(header), 0);

    while (offset < st.st_size)
    {
        sent = sendfile(clientSocket, imagefd, &offset, st.st_size - offset);
        if (sent <= 0)
            break;
    }

    close(imagefd);
}

void handle_client(int clientSocket)
{
    char buffer[BUFFER_SIZE], verb[8], version[16], path[256], filename[512];
    char *firstLine;

    recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    firstLine = strtok(buffer, "\r\n");
    if (!firstLine)
        return;

    sscanf(firstLine, "%s %s %s", verb, path, version);

    if (debug)
    {
        printf("HTTP Request\n");
        printf("Verbo: %s\n", verb);
        printf("Ruta: %s\n", path);
        printf("Versión: %s\n", version);
    }

    if (strcmp(path, "/") == 0)
    {
        const char *html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Servidor C funcionando</h1>";
        send(clientSocket, html, strlen(html), 0);
    }
    else if (strcmp(path, "/Ping") == 0 || strcmp(path, "/ping") == 0)
    {
        const char *pong = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\npong";
        send(clientSocket, pong, strlen(pong), 0);
    }
    else
    {
        snprintf(filename, sizeof(filename), ".%s.jpg", path);
        filename[strcspn(filename, "? ")] = 0;

        if (access(filename, F_OK) == 0)
        {
            send_image(clientSocket, filename);
        }
        else
        {
            const char *notfound = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nImagen no encontrada.";
            send(clientSocket, notfound, strlen(notfound), 0);
        }
    }

    close(clientSocket);
}

void *handle_client_threadmode(void *arg)
{
    int clientSocket = *((int *)arg);
    free(arg);

    handle_client(clientSocket);

    return NULL;
}
