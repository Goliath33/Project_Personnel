#include "main.h"

#define PORT 80
#define WEBROOT "/home/goliath/Project/Project_holbertonschool/webserver/web/"
#define BUFFER_SIZE 1024

/**
 * Prints an error message to stderr and exits with status 1.
 *
 * @param message The error message to print and exit with.
 */
void fatal(char *message)
{
    perror(message);
    exit(1);
}

int main(void)
{
    int sockfd, new_sockfd;
    struct sockaddr_in host_addr, client_addr;
    socklen_t address_len;

    printf("Listening on port %d...\n", PORT);

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fatal("socket creation failed");
    }

    // Set up the socket address
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    inet_ntoa(host_addr.sin_addr, host_addr.sin_addr_len);

    // Loop to handle connections
    while (1)
    {
        new_sockfd = accept(sockfd, &client_addr, &address_len);
        if (new_sockfd == -1)
        {
            fatal("accept failed");
        }

        // Handle the connection
        handle_connection(new_sockfd, &client_addr);

    }
    return 0;
}


void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
    char buffer[BUFFER_SIZE];
    ssize_t numbytes;

    // Open the index.html file
    int fd = open(WEBROOT "index.html", O_RDONLY);
    if (fd == -1)
    {
        fatal("open failed");
    }

    // Read the file into the buffer
    numbytes = read(fd, buffer, BUFFER_SIZE);
    if (numbytes == -1)
    {
        fatal("read failed");
    }

    // Send the file to the client
    if (send(sockfd, buffer, numbytes, 0) == -1)
    {
        fatal("send failed");
    }

    close(fd); // Close the file
}