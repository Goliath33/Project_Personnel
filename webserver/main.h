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

// Inclusions de bibliothèques supplémentaires
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>

// Definitions
#define SERVER_PORT 80
#define WEB_ROOT "/home/goliath/Project/Project_holbertonschool/webserver/web/holbertonschool/"
#define BUFFER_SIZE 1024

// Prototypes
int main(int argc, char const *argv[]);                                         //main appele le serveur web    (1er parametre)
void serve_php(int sockfd, char *file_path);                                    //serveur php                   (2eme parametre)
void serve_file(int sockfd, char *file_path);                                   //serveur file                  (3eme parametre)
void serve_request(int sockfd, struct sockaddr_in *client_addr_ptr);            //serveur request               (4eme parametre)
void serve_http(int sockfd, struct sockaddr_in *client_addr_ptr);               //serveur http                  (5eme parametre)
void serve_https(int sockfd, struct sockaddr_in *client_addr_ptr);              //serveur https                 (6eme parametre)
void fatal(const char *message);                                                //fatale message d'erreur       (7eme parametre)
void handle_request(int sockfd);                                                //handle request                (8eme parametre)
void handle_proxy_request(int sockfd);                                          //handle proxy request          (9eme parametre)
void *thread_function(void *arg);                                               //thread function               (10eme parametre)
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr);        //handle connection             (11eme parametre)
void serve_connection(int sockfd, struct sockaddr_in *client_addr_ptr);         //serve connection              (12eme parametre)
void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr);   //serve connection proxy        (13eme parametre)


#endif // MAIN_H
