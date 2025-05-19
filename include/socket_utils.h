#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT 8080

int initialize_server(struct sockaddr_in* address);

#endif
