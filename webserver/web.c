/**
 * @file web.c - webserver html + php
 * AUTHOR : GOLIATH
 * VERSION : 1.1.0
 */

#include "main.h"

void serve_php(int sockfd, char *file_path)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_file(int sockfd, char *file_path)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);
    return;
}

void serve_request(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);
    return;
}

void serve_http(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char request[BUFFER_SIZE];
    ssize_t bytes_received = recv(sockfd, request, sizeof(request) - 1, 0);
    if (bytes_received == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "recv: %s", strerror(errno));
        perror(error_message);
        return;
    }

    request[bytes_received] = '\0';
    printf("Received request: %s\n", request);

    int php_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (php_sockfd < 0)
    {
        perror("socket creation failed");
        return;
    }
    struct sockaddr_in php_addr;
    php_addr.sin_family = 2;
    php_addr.sin_port = htons(9000);
    php_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(php_sockfd, (struct sockaddr *)&php_addr, sizeof(php_addr)) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "connect: %s", strerror(errno));
        perror(error_message);
        return;
    }
    if (send(php_sockfd, request, strlen(request), 0) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "send: %s", strerror(errno));
        perror(error_message);
        return;
    }

    char response[BUFFER_SIZE];
    ssize_t bytes_received_from_php = recv(php_sockfd, response, sizeof(response) - 1, 0);
    if (bytes_received_from_php == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "recv: %s", strerror(errno));
        perror(error_message);
        return;
    }
    response[bytes_received_from_php] = '\0';

    if (send(sockfd, response, strlen(response), 0) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "send: %s", strerror(errno));
        perror(error_message);
        return;
    }

    close(sockfd);
    close(php_sockfd);
}

void serve_https(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);
    return;
    // TODO: implement https
    // serve_file(sockfd, file_path);
    // return;
    // TODO: implement https
    // serve_file(sockfd, file_path);
    // return;

    // serve_file(sockfd, file_path);
    // return;
}

void fatal(const char *message)
{
    char error_message[1024];
    snprintf(error_message, sizeof(error_message), "%s: %s", message, strerror(errno));
    perror(error_message);
    exit(1);
    return;
}

void handle_request(int sockfd)
{
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, &sockfd);
    pthread_detach(thread);
    return;

    // TODO: implement proxy
}

void handle_proxy_request(int sockfd)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void *thread_function(void *arg)
{
    int client_sockfd = *(int *)arg;
    char request[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_sockfd, request, sizeof(request) - 1, 0);
    if (bytes_received == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "recv: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }

    request[bytes_received] = '\0';

    int php_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (php_sockfd < 0)
    {
        perror("socket creation failed");
        return NULL;
    }
    struct sockaddr_in php_addr;
    php_addr.sin_family = 2;
    php_addr.sin_port = htons(9000);
    php_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(php_sockfd, (struct sockaddr *)&php_addr, sizeof(php_addr)) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "connect: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }
    if (send(php_sockfd, request, strlen(request), 0) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "send: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }

    char response[BUFFER_SIZE];
    ssize_t bytes_received_from_php = recv(php_sockfd, response, sizeof(response) - 1, 0);
    if (bytes_received_from_php == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "recv: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }
    response[bytes_received_from_php] = '\0';

    if (send(client_sockfd, response, strlen(response), 0) == -1)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "send: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }

    close(client_sockfd);
    close(php_sockfd);
    return NULL;
}

void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t numbytes;

    printf("Handling connection from %s\n", inet_ntoa(client_addr_ptr->sin_addr));

    char *file_path;
    if (access(WEB_ROOT "index.html", F_OK) != -1)
    {
        file_path = WEB_ROOT "index.html";
    }
    else if (access(WEB_ROOT "index.php", F_OK) != -1)
    {
        file_path = WEB_ROOT "index.php";
    }
    else
    {
        fatal("index file not found");
    }

    fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        fatal("open");
    }

    while ((numbytes = read(fd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, numbytes, 0);
    close(fd);
    close(sockfd);
}

void serve_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    serve_file(sockfd, WEB_ROOT "http.php");
    serve_file(sockfd, WEB_ROOT "https.php");
    serve_php(sockfd, "some_file.php");
}

void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);
}

int main(int argc, char const *argv[])
{
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fp;
    char buffer[256];

    // Créez un socket pour écouter les connexions entrantes
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("Erreur lors de l'ouverture d'un socket");

    // Initialisation du serveur et traitement des requêtes HTTP

    // Initialisation du serveur
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        perror("Erreur lors de la liaison du socket");

    // Lancement du serveur
    listen(sockfd, 5);
    while (1)
    {
        socklen_t clilen;
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            perror("accept");
        handle_connection(newsockfd, &cli_addr);
    }
    close(sockfd);
    return 0;   
}
