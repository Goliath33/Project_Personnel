#include "main.h"
/**
 * Prints an error message to stderr and exits with status 1.
 *
 * @param message The error message to print and exit with.
 */

// defines
#define PORT 80
#define WEBROOT "/home/goliath/Project/Project_holbertonschool/webserver/web/"
#define BUFFER_SIZE 1024

// function prototypes
void fatal(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fatal("socket creation failed");
    }
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(struct sockaddr)) < 0)
    {
        fatal("binding failed");
    }
    if (listen(sockfd, 20) < 0)
    {
        fatal("listening failed");
    }
    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int new_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_len);
        if (new_sockfd < 0)
        {
            fatal("accept failed");
        }
        void *thread_arg = (void *)new_sockfd;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_function, thread_arg);
    }
    close(sockfd);
    return 0;
}

void *thread_function(void *arg)
{
    int new_sockfd = *(int *)arg;
    char request[1024];
    ssize_t bytes_received = recv(new_sockfd, request, sizeof(request) - 1, 0);
    if (bytes_received == -1)
    {
        perror("recv");
        return NULL;
    }

    // Ajoutez un caractère de fin de chaîne à la requête
    request[bytes_received] = '\0';

    printf("Received HTTP request: %s\n", request);

    // Envoyez la requête au serveur PHP-FPM
    int php_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in php_addr;
    php_addr.sin_family = AF_INET;
    php_addr.sin_port = htons(9000); // Assurez-vous que c'est le port sur lequel PHP-FPM écoute
    php_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(php_sockfd, (struct sockaddr *)&php_addr, sizeof(php_addr)) == -1)
    {
        perror("connect");
        return NULL;
    }
    if (send(php_sockfd, request, strlen(request), 0) == -1)
    {
        perror("send");
        return NULL;
    }

    // Recevez la réponse du serveur PHP-FPM
    char response[1024];
    ssize_t bytes_received_from_php = recv(php_sockfd, response, sizeof(response) - 1, 0);
    if (bytes_received_from_php == -1)
    {
        perror("recv");
        return NULL;
    }
    response[bytes_received_from_php] = '\0';

    printf("Received PHP response: %s\n", response);

    // Envoyez la réponse au client
    if (send(new_sockfd, response, strlen(response), 0) == -1)
    {
        perror("send");
        return NULL;
    }

    close(new_sockfd);
    close(php_sockfd);
    return NULL;
}

void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t numbytes;

    printf("Handling connection from %s\n", inet_ntoa(client_addr_ptr->sin_addr));

    // Open the index.html file
    fd = open(WEBROOT "index.html", O_RDONLY);
    if (fd == -1)
        fatal("open");
    // Send the HTTP header
    send_header(sockfd, client_addr_ptr);
    // Send the contents of the file
    while ((numbytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    send(sockfd, buffer, numbytes, 0);
    close(fd);
    close(sockfd);
    return;

    // Open the index.php file
    fd = open(WEBROOT "index.php", O_RDONLY);
    if (fd == -1)
        fatal("open");
    // Send the HTTP header
    send_header(sockfd, client_addr_ptr);   
    // Send the contents of the file
    while ((numbytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    send(sockfd, buffer, numbytes, 0);
    close(fd);
    close(sockfd);
    return;
}