/**
 * Simple Web Server html + php
 * AUTHOR : GOLIATH
 * VERSION : 1.0.0
 */

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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


// Definitions
#define SERVER_PORT 80
#define WEB_ROOT "/home/goliath/Project/Project_holbertonschool/webserver/web/"
#define BUFFER_SIZE 1024

// Prototypes
int main(void);
void serve_php(int sockfd, char *file_path);
void serve_http(int sockfd, struct sockaddr_in *client_addr_ptr);
void serve_https(int sockfd, struct sockaddr_in *client_addr_ptr);
void fatal(const char *message);
void handle_request(int sockfd);
void handle_proxy_request(int sockfd);
void serve_file(int sockfd, char *file_path);
void serve_php(int sockfd, char *file_path);
void *thread_function(void *arg);
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr);


#endif // MAIN_H
