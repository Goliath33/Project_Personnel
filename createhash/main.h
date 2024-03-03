#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <openssl/sha.h>
#include <time.h>

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
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    char operation[MAX_OPERATION_LENGTH];
    char passphrase[MAX_KEY_LENGTH];

    int choice;

    printf("Enter 1 to Encrypt / 2 to Decrypt");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Encrypting File\n");
    }
    else if (choice == 2)
    {
        printf("Decrypting File\n");
    }
    else
    {
        printf("Invalid Choice\n");
        return 1;
    }

    printf("Enter input filename: ");
    scanf("%s", inputFilename);
    printf("Enter output filename: ");
    scanf("%s", outputFilename);
    printf("Enter key: ");
    scanf("%s", key);
    printf("Enter operation (encrypt/decrypt): ");
    scanf("%s", operation);
    printf("Enter passphrase: ");
    scanf("%s", passphrase);

    if (strcmp(operation, "encrypt") == 0)
    {
        encryptFile(inputFilename, outputFilename, key);
    }
    else if (strcmp(operation, "decrypt") == 0)
    {
        decryptFile(inputFilename, outputFilename, key);
    }

    return 0;
}

// Function to get user input
int main();
// Function to create a SHA256 hash from a passphrase
void createSHA256Hash(char *passphrase, unsigned char *hash);

// Function to encrypt or decrypt a file
int processFile(char *inputFilename, char *outputFilename, char *key, char *operation);
int encryptFile(char *inputFilename, char *outputFilename, char *key);
int decryptFile(char *inputFilename, char *outputFilename, char *key);

#endif