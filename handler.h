#ifndef HANDLER_H
#define HANDLER_H

void handle_client(int clientSocket);
void *handle_client_threadmode(void *arg);

#endif