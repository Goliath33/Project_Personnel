/**
 * @file web.c - webserver html + php
 * AUTHOR : GOLIATH
 * VERSION : 1.1.0
 */

#include "main.h"

/**
 * Initializes a TCP socket, binds it to the server port,
 * listens for incoming connections, and accepts new
 * connections by creating new sockets. The new sockets
 * are passed to handle_connection() to manage the
 * individual client connections.
 */

const char *extensions[] = {".html", "text/html", ".php", "application/x-httpd-php", ".jpg", "image/jpeg",
                            ".png", "image/png", ".gif", "image/gif", ".css", "text/css", ".js", "application/javascript",
                            ".ico", "image/vnd.microsoft.icon", ".pdf", "application/pdf", ".txt", "text/plain",
                            ".zip", "application/zip", ".mp3", "audio/mpeg", ".mp4", "video/mp4", ".avi",
                            "video/x-msvideo", ".mov", "video/quicktime", ".mpg", "video/mpeg", ".mpeg",
                            "video/mpeg", ".wmv", "video/x-ms-wmv", ".svg", "image/svg+xml"};

const char *mime_types[] = {"text/html", "text/html", "application/x-httpd-php", "application/x-httpd-php",
                            "image/jpeg", "image/jpeg", "image/png", "image/png", "image/gif", "image/gif",
                            "text/css", "text/css", "application/javascript", "application/javascript",
                            "image/vnd.microsoft.icon", "image/vnd.microsoft.icon", "application/pdf",
                            "application/pdf", "text/plain", "text/plain", "application/zip",
                            "application/zip", "audio/mpeg", "audio/mpeg", "video/mp4", "video/mp4",
                            "video/x-msvideo", "video/x-msvideo", "video/quicktime", "video/mpeg",
                            "video/mpeg", "video/mpeg", "video/x-ms-wmv", "image/svg+xml", "image/svg+xml"};

int main(int argc, char const *argv[])
{
    printf("Web Server\n");
    printf("Simple Web Server html + php\n");
    printf("AUTHOR : GOLIATH\n");
    printf("VERSION : 1.1.0\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) // Pas besoin de comparer avec -1 car si le socket n'est pas crée alors on rencontrera un échec.
    {
        fatal("socket creation failed");
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        fatal("binding failed");
    }

    if (listen(sockfd, 20) < 0) // Pas besoin de comparer avec -1 car si la connexion n'est pas établie alors on rencontrera un échec.
    {
        fatal("listening failed");
    }

    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int new_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &addr_size); // Accepte la connexion du client.

        if (new_sockfd < 0) // Si une erreur se produit pendant l'acceptation de la connexion.
            perror("Accept failed");

        handle_connection(new_sockfd, &client_address); // Gère cette nouvelle connexion.
    }

    close(sockfd); // Ferme le socket principal.
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PROTOTYPES SUPPLÉMENTAIRES*/ ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    return;
}

void handle_request(int sockfd)
{
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, &sockfd);
    pthread_detach(thread);
    return;
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

void fatal(const char *message)
{
    char error_message[1024];
    snprintf(error_message, sizeof(error_message), "%s: %s", message, strerror(errno));
    perror(error_message);
    exit(1);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*SERVER CONNECTION*/ /////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void serve_php(int sockfd, char *file_path)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    {
        send(sockfd, buffer, num_bytes, 0);
    }
    close(sockfd);

    return;
}

void serve_file(int sockfd, char *file_path, char *request)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "fopen: %s", strerror(errno));
        perror(error_message);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    while ((num_bytes = fread(buffer, 1, sizeof(buffer), file)) > -0) // Modification faite ici. La comparaison est maintenant correcte et respecte les bonnes pratiques d'erreur de fread()
    {
        if (send(sockfd, buffer, num_bytes, 0) < 0)
        {
            perror("Error sending data");
            break;
        }
    }

    if (ferror(file)) // Si une erreur survient lors de la lecture du fichier
    {
        perror("fread() error occurred while reading file");
    }

    fclose(file);
    close(sockfd);

    return;
}

void serve_http(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    int num_bytes;
    while ((num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        // Null terminate the string to ensure valid C-string
        buffer[num_bytes] = '\0';
        printf("Received request: %s\n", buffer);
    }
    close(sockfd);
    return;
}

void serve_https(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    int num_bytes;
    while ((num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        // Null terminate the string to ensure valid C-string
        buffer[num_bytes] = '\0';
        printf("Received request: %s\n", buffer);
    }
    close(sockfd);
    return;
}

void serve_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    int num_bytes;

    while ((num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        // Null terminate the string to ensure valid C-string
        buffer[num_bytes] = '\0';
        printf("Received request: %s\n", buffer);
        char *method = strtok(buffer, " ");
        if (strcasecmp("GET", method) == 0 || strcasecmp("POST", method) == 0)
        {
            // Handle HTTP GET or POST requests
            serve_connection_http(sockfd, client_addr_ptr);
        }
        else if (strcasecmp("CONNECT", method) == 0)
        {
            // Handle HTTP CONNECT requests
            serve_connection_https(sockfd, client_addr_ptr);
        }
        else if (strncasecmp("GET ", buffer, 4) == 0 || strncasecmp("POST", buffer, 4) == 0)
        {
            // Handle other HTTP methods or file requests
            serve_connection_file(sockfd, client_addr_ptr);
        }
    }

    if (num_bytes < 0)
    {
        perror("Error reading from socket");
    }

    close(sockfd);
}

void serve_connection_https(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_connection_http(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_connection_php(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_connection_file(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_connection_error(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}

void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
        send(sockfd, buffer, num_bytes, 0);
    close(sockfd);

    return;
}
