
// defines
#define PORT 80
#define WEBROOT "/home/goliath/Project/Project_holbertonschool/webserver/web/"
#define BUFFER_SIZE 1024

// function prototypes
void fatal(char *message)
{
    perror(message);
    exit(1);
}

int main(void)
{
    int sockfd, new_sockfd, yes = 1;
    struct sockaddr_in host_addr, client_addr;
    socklen_t sin_size;
    printf("Accepting web requests on port %d\n", PORT);
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        fatal("in socket");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        fatal("setting socket option SO_REUSEADDR");
    host_addr.sin_family = AF_INET;         // Ordre des octets de l'hôte.
    host_addr.sin_port = htons(PORT);       // Ordre des octets du réseau (entier court).
    host_addr.sin_addr.s_addr = 0;          // Remplir automatiquement l'IP.
    memset(&(host_addr.sin_zero), '\0', 8); // Reste de la structure à 0.
    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        fatal("binding to socket");
    if (listen(sockfd, 20) == -1)
        fatal("listening on socket");
    while (1)
    { // Boucle d'acceptation des connexions entrantes.
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (new_sockfd == -1)
            fatal("accepting connection");

        handle_connection(new_sockfd, &client_addr); // Ajout de l'appel à handle_connection

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, thread_function, &new_sockfd) != 0)
            fatal("creating thread");

        // Vous pouvez également détacher le thread si vous ne voulez pas attendre qu'il se termine.
        pthread_detach(thread_id);
    }
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
    char buffer[BUFFER_SIZE];
    ssize_t numbytes;
    int fd;

    printf("Handling connection from %s\n", inet_ntoa(client_addr_ptr->sin_addr));

    // Open the index.html file
    fd = open(WEBROOT "index.html", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Open the index.php file
    fd = open(WEBROOT "index.php", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Read the file into the buffer
    numbytes = read(fd, buffer, BUFFER_SIZE);
    if (numbytes == -1)
    {
        perror("read");
        close(fd); // Close the file
        return;
    }

    // Send the file to the client
    if (send(sockfd, buffer, numbytes, 0) == -1)
    {
        perror("send");
        close(fd); // Close the file
        return;
    }

    close(fd); // Close the file
}
