#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "config.h"

int debug = 0;

int main(int argc, char *argv[])
{
    int modo;
    int puerto;
    int serverSocket;

    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <modo: 0|1|2> <puerto> [debug]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    modo = atoi(argv[1]);
    puerto = atoi(argv[2]);

    if (argc >= 4 && strcmp(argv[3], "-d") == 0)
    {
        debug = 1;
        printf("Debug activado\n");
    }

    serverSocket = setup_server_socket(puerto);
    printf("Servidor escuchando en puerto %d (modo %d)...\n", puerto, modo);

    switch (modo)
    {
    case 0:
        run_thread_mode(serverSocket);
        break;
    case 1:
        run_select_mode(serverSocket);
        break;
    case 2:
        run_pool_mode(serverSocket);
        break;
    default:
        fprintf(stderr, "Modo inválido. Use 0, 1 o 2.\n");
        return 1;
    }

    return 0;
}
