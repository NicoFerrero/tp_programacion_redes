#ifndef SERVER_H
#define SERVER_H

#define DEFAULT_PORT 8080
#define MODE_THREAD 0
#define MODE_SELECT 1
#define MODE_POOL 2

int setup_server_socket(int port);
void run_thread_mode(int serverSocket);
void run_select_mode(int serverSocket);
void run_pool_mode(int serverSocket);

#endif
