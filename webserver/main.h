#ifndef MAIN_H
#define MAIN_H

// Inclusions de bibliothèques standard
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>

// Déclaration de vos fonctions
void fatal(const char *message);
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr);

#endif // MAIN_H
