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
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>

// Definitions
#define SERVER_PORT 80
#define WEB_ROOT "/home/goliath/Projet/Project_holbertonschool/webserver/web/"
#define BUFFER_SIZE 1024

// Prototypes principaux
int main(int argc, char const *argv[]); // main appelant le serveur web (premier parametre)
void fatal(const char *message);        // affiche un message d'erreur

// Gestion des threads
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
//void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr); // serveur connection proxy

// Gestion des requêtes
void send_file(int sockfd, char *file_path);
void send_php(int sockfd, char *file_path);


// Gestion des connexions et demandes
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr); 
void handle_http_request(int sockfd);
//void handle_request(int sockfd);                                         // gérer la requête


// void handle_error(int client_socket);

// Gestion des fichiers
//void handle_file(int client_socket, const char *file_path);

// Gestion du proxy
//void handle_proxy(int client_socket, char *file_path);                   // gérer la requête proxy

// Gestion des erreurs
void send_404(int sockfd);
void send_500(int sockfd);
void send_502(int sockfd);
void send_503(int sockfd);
void send_504(int sockfd);
void send_505(int sockfd);
void send_507(int sockfd);
void send_508(int sockfd);
void send_509(int sockfd);
void send_510(int sockfd);  

#endif // MAIN_H