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

void fatal(const char *message)
{

    char error_message[1024];
    snprintf(error_message, sizeof(error_message), "%s: %s", message, strerror(errno));
    perror(error_message);
    exit(1);
    return;
}

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

    int php_sockfd;

    while (1)
    {
        ssize_t bytes_received = recv(client_sockfd, request, sizeof(request) - 1, 0);
        if (bytes_received <= 0)
        {
            break;
        }

        request[bytes_received] = '\0';

        send(php_sockfd, request, strlen(request), 0);

        char response[BUFFER_SIZE];
        ssize_t bytes_sent = recv(php_sockfd, response, sizeof(response) - 1, 0);
        if (bytes_sent <= 0)
        {
            break;
        }

        send(client_sockfd, response, bytes_sent, 0);
    }

    request[bytes_received] = '\0';

    php_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (php_sockfd < 0)
    {
        perror("socket creation failed");
        return NULL;
    }

    struct sockaddr_in php_address;
    memset(&php_address, 0, sizeof(php_address));
    php_address.sin_family = AF_INET;
    php_address.sin_port = htons(SERVER_PORT);
    php_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(php_sockfd, (struct sockaddr *)&php_address, sizeof(php_address)) < 0)
    {
        char error_message[1024];
        snprintf(error_message, sizeof(error_message), "connect: %s", strerror(errno));
        perror(error_message);
        return NULL;
    }

    serve_php(php_sockfd, request);
    close(php_sockfd);
    close(client_sockfd);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*SERVER CONNECTION*/ ////////////////////////////////////////////////////////////////////////////////////////////
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
    ssize_t num_bytes;

    while ((num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {                             // Utilisez recv au lieu de read
        buffer[num_bytes] = '\0'; // Ajoutez ce qui suit pour terminer la chaîne par un caractère nul
        printf("Received: %s\n", buffer);

        if (send(sockfd, buffer, num_bytes, 0) < 0)
        { // Envoyer à nouveau le message au client
            perror("Error sending response to client");
            break;
        }
    }

    close(sockfd);
}

void serve_connection_https(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(sockfd, buffer, num_bytes) < 0)
        {
            perror("Error sending response to client");
            break;
        }
    }

    close(sockfd);
}

void serve_connection_http(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(sockfd, buffer, num_bytes) < -1)
        {
            perror("Error sending response to client");
            break;
        }
    }

    close(sockfd);
}
void serve_connection_php(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(sockfd, buffer, num_bytes) < -1)
        {
            perror("Error sending response to client");
            break;
        }
    }

    close(sockfd);
}

void serve_connection_file(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    { // Le signe égal est mis en majuscule pour correspondre au code précédent.
        if (write(sockfd, buffer, num_bytes) < 0)
        { // Tous les points de suspension sont remplacés par leur valeur exacte et correctement placée (-1).
            perror("Error sending response to client");
            break;
        }
    }

    close(sockfd);
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

// void serve_connection_proxy(int sockfd, struct sockaddr_in *client_addr_ptr)
//{
// char buffer[BUFFER_SIZE];
// ssize_t num_bytes;
// while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
// send(sockfd, buffer, num_bytes, 0);
// close(sockfd);

// return;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*GESTION DES REQUETES*/ /////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void send_file(int sockfd, char *file_path)
{
    int fd;
    ssize_t numbytes;
    char buffer[BUFFER_SIZE];
    fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        send_404(sockfd);
        return;
    }
    while ((numbytes = read(fd, buffer, BUFFER_SIZE)) > 0)
        if (send(sockfd, buffer, numbytes, 0) < 0)
        {
            perror("Error sending response to client");
            break;
        }
    close(fd);

    return;
}

void send_php(int sockfd, char *file_path)
{
    send_file(sockfd, file_path);
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PROTOTYPES SUPPLÉMENTAIRES*/ ///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Handles a new incoming client connection.
 * Reads the requested file from disk and sends it to the client.
 * If no index file exists, sends a 404 response.
 */
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
    int err = pthread_create(&thread, NULL, thread_function, &sockfd);
    if (err != 0)
    {
        send_500(sockfd);
        return;
    }
    return;
}

void handle_proxy_request(int sockfd)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = read(sockfd, buffer, BUFFER_SIZE)) > 0)
    {
        send(sockfd, buffer, num_bytes, 0);
    }
    if (num_bytes == -1)
    {
        send_500(sockfd);
        return;
    }
    close(sockfd);
    return;
}

void handle_http_request(int client_socket)
{
    char request[1024];
    ssize_t num_bytes = recv(client_socket, request, sizeof(request), 0);
    if (num_bytes == -1)
    {
        send_500(client_socket);
        close(client_socket);
        return;
    }

    FILE *css_file = fopen("*.css", "r");
    if (css_file)
    {
        fseek(css_file, 0, SEEK_END);
        long file_size = ftell(css_file);
        fseek(css_file, 0, SEEK_SET);
        char *file_content = malloc(file_size + 1);
        fread(file_content, file_size, 1, css_file);
        fclose(css_file);
        file_content[file_size] = '\0';

        char http_response[1024];
        sprintf(http_response, "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: %ld\r\n\r\n%s",
                file_size, file_content);
        send(client_socket, http_response, strlen(http_response), 0);

        free(file_content);
    }
    else
    {
        send_404(client_socket);
    }

    close(client_socket);
}

void handle_file(int client_socket, const char *file_path)
{
    int fd;
    ssize_t numbytes;
    char buffer[BUFFER_SIZE];

    if (client_socket < 0 || client_socket > INT_MAX)
    {
        fprintf(stderr, "Invalid socket file descriptor: %d\n", client_socket);
        return;
    }

    /* Open the file */
    fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        char error[BUFFER_SIZE];
        snprintf(error, sizeof(error), "Failed to open %s: %s\n",
                 file_path, strerror(errno));
        send(client_socket, error, strlen(error), 0);
        return;
    }

    /* Read and send the file content */
    while ((numbytes = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (send(client_socket, buffer, numbytes, 0) == -1)
        {
            perror("Error sending response");
            break;
        }
    }

    /* Handle errors during read */
    if (numbytes == -1)
    {
        char error[BUFFER_SIZE];
        snprintf(error, sizeof(error), "Failed to read %s: %s\n",
                 file_path, strerror(errno));
        send(client_socket, error, strlen(error), 0);
    }

    /* Close the file descriptor */
    if (close(fd) == -1)
    {
        perror("Error closing file");
    }
}

void handle_proxy(int client_socket, char *file_path)
{
    handle_file(client_socket, file_path);
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*GESTION DES ERREURS*/ //////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void send_404(int sockfd)
{
    send(sockfd, "HTTP/1.1 404 Not Found\r\n\r\n", 26, 0);
    return;
}

void send_500(int sockfd)
{
    send(sockfd, "HTTP/1.1 500 Internal Server Error\r\n\r\n", 40, 0);
    return;
}

void send_502(int sockfd)
{
    send(sockfd, "HTTP/1.1 502 Bad Gateway\r\n\r\n", 30, 0);
    return;
}

void send_503(int sockfd)
{
    send(sockfd, "HTTP/1.1 503 Service Unavailable\r\n\r\n", 40, 0);
    return;
}

void send_504(int sockfd)
{
    send(sockfd, "HTTP/1.1 504 Gateway Timeout\r\n\r\n", 33, 0);
    return;
}

void send_505(int sockfd)
{
    send(sockfd, "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n", 46, 0);
    return;
}

void send_507(int sockfd)
{
    send(sockfd, "HTTP/1.1 507 Insufficient Storage\r\n\r\n", 37, 0);
    return;
}

void send_508(int sockfd)
{
    send(sockfd, "HTTP/1.1 508 Loop Detected\r\n\r\n", 30, 0);
    return;
}

void send_509(int sockfd)
{
    send(sockfd, "HTTP/1.1 509 Bandwidth Limit Exceeded\r\n\r\n", 46, 0);
    return;
}

void send_510(int sockfd)
{
    send(sockfd, "HTTP/1.1 510 Not Extended\r\n\r\n", 29, 0);
    return;
}
