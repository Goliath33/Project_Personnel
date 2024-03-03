#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <openssl/sha.h>
#include <time.h>

// Definitions
#define BUFFER_SIZE 1024
#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32
#define SHA256_DIGEST_LENGTH 32
#define MAX_FILENAME_LENGTH 1024
#define MAX_KEY_LENGTH 1024
#define MAX_OPERATION_LENGTH 1024

// Prototypes
int main()
{
    char inputFilename[MAX_FILENAME_LENGTH];
    char outputFilename[MAX_FILENAME_LENGTH];
    char key[MAX_KEY_LENGTH];
    char operation[MAX_OPERATION_LENGTH];
    int option;
    while (1)
    {
        printf("1. Encrypt a file\n");
        printf("2. Decrypt a file\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);
        if (option == 3)    
            break;
            else if (option < 1 || option > 3)
                continue;
                else
                    printf("\n");
                    switch (option)
                    {
                    case 1:
                        printf("Enter input filename: ");
                        scanf("%s", inputFilename);
                        printf("Enter output filename: ");
                        scanf("%s", outputFilename);
                        printf("Enter key: ");
                        scanf("%s", key);
                        printf("Enter operation: ");
                        scanf("%s", operation);
                        printf("Encrypting...\n");
                        encryptFile(inputFilename, outputFilename, key);
                        break;
                        
                    case 2:
                        printf("Enter input filename: ");
                        scanf("%s", inputFilename);
                        printf("Enter output filename: ");
                        scanf("%s", outputFilename);
                        printf("Enter key: ");
                        scanf("%s", key);
                        decryptFile(inputFilename, outputFilename, key);
                        break;
                    case 3:
                        exit(0);
                    default:
                        printf("Invalid option\n");
                        break;

                
                    }
        switch (option)
        {
        case 1:
            printf("Enter input filename: ");
            scanf("%s", inputFilename);
            printf("Enter output filename: ");
            scanf("%s", outputFilename);
            printf("Enter key: ");
            scanf("%s", key);
            printf("Enter operation: ");
            scanf("%s", operation);
            printf("Encrypting...\n");
            encryptFile(inputFilename, outputFilename, key);
            break;
            
        case 2:
            printf("Enter input filename: ");
            scanf("%s", inputFilename);
            printf("Enter output filename: ");
            scanf("%s", outputFilename);
            printf("Enter key: ");
            scanf("%s", key);
            decryptFile(inputFilename, outputFilename, key);
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid option\n");
            break;
        }           
    }
    return 0;
}

// Function to get user input
int main();
void fatal(char *msg);

// Function to create a SHA256 hash from a passphrase
void createSHA256Hash(char *passphrase, unsigned char *hash);
void printHash(unsigned char *hash);

// Function to encrypt or decrypt a file
int processFile(char *inputFilename, char *outputFilename, char *key, char *operation);
int encryptFile(char *inputFilename, char *outputFilename, char *key);
int decryptFile(char *inputFilename, char *outputFilename, char *key);

#endif