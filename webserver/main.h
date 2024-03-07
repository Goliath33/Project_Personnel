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

// Prototypes principaux
int main(int argc, char const *argv[]); // main appelant le serveur web (premier parametre)
void fatal(const char *message);        // affiche un message d'erreur
void *thread_function(void *arg);       // fonction de thread

// Serveurs spécifiques
void serve_php(int sockfd, char *file_path);                                  // serveur php
void serve_file(int sockfd, char *file_path, char *request);                  // serveur fichier
void serve_http(int sockfd, struct sockaddr_in *client_addr_ptr);             // serveur http
void serve_https(int sockfd, struct sockaddr_in *client_addr_ptr);            // serveur https
void serve_connection(int sockfd, struct sockaddr_in *client_addr_ptr);       // serveur connection
void serve_connection_https(int sockfd, struct sockaddr_in *client_addr_ptr); // serveur connection https
void serve_connection_http(int sockfd, struct sockaddr_in *client_addr_ptr);  // serveur connection http
void serve_connection_php(int sockfd, struct sockaddr_in *client_addr_ptr);   // serveur connection php
void serve_connection_file(int sockfd, struct sockaddr_in *client_addr_ptr);  // serveur connection file
void serve_connection_error(int sockfd, struct sockaddr_in *client_addr_ptr); // serveur connection error
void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr); // serve connection proxy

// Gestion des connexions et demandes
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr); // gérer la connexion
void handle_request(int sockfd);                                         // gérer la requête
void handle_proxy_request(int sockfd);                                   // gérer la requête proxy


#endif // MAIN_H