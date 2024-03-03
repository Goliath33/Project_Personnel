#include "main.h"

void fatal(char *msg)
{
    printf("Fatal error: %s\n", msg);
}

void createSHA256Hash(char *passphrase, unsigned char *hash)
{
    if (strlen(passphrase) > SHA256_DIGEST_LENGTH)
    {
        fatal("Passphrase is too long");
    }
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, passphrase, strlen(passphrase));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        snprintf(hash + (i * 2), sizeof(hash) - (i * 2), "%02x", hash[i]);
    }

    hash[SHA256_DIGEST_LENGTH * 2] = '\0';
}

void printHash(unsigned char *hash)
{
    printf("\nHash: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", hash[i]);
    }
}

int processFile(char *inputFilename, char *outputFilename, char *key, char *operation)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    createHash(key, hash);

    int hashIndex = 0;
    unsigned char inputByte;
    unsigned char outputByte;
    FILE *inFile;
    FILE *outFile;

    inFile = fopen(inputFilename, "rb");
    outFile = fopen(outputFilename, "wb");

    if (inFile == NULL || outFile == NULL)
    {
        fatal("Error in opening file");
    }

    printf("%sing %s...\n", operation, inputFilename);

    while (!feof(inFile))
    {
        inputByte = fgetc(inFile);
        if (feof(inFile))
        {
            break;
        }
        outputByte = inputByte ^ hash[hashIndex++ % SHA256_DIGEST_LENGTH];
        fputc(outputByte, outFile);
    }

    printf("End of File");

    fclose(inFile);
    fclose(outFile);

    printHash(hash);
}

int encryptFile(char *inputFilename, char *outputFilename, char *key)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    createSHA256Hash(key, hash);

    int hashIndex = 0;
    unsigned char inputByte;
    unsigned char encryptedByte;
    FILE *inFile;
    FILE *outFile;

    inFile = fopen(inputFilename, "rb");
    outFile = fopen(outputFilename, "wb");

    if (inFile == NULL || outFile == NULL)
    {
        fatal("Error in opening file");
        return 1;
    }

    printf("\nFile Opened, Encrypting");

    while ((inputByte = fgetc(inFile)) != EOF)
    {
        encryptedByte = inputByte ^ hash[hashIndex++ % SHA256_DIGEST_LENGTH];
        fputc(encryptedByte, outFile);
    }

    printf("End of File");

    fclose(inFile);
    fclose(outFile);

    return 0;
}

int decryptFile(char *inputFilename, char *outputFilename, char *key)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    createSHA256Hash(key, hash);

    int hashIndex = 0;
    unsigned char inputByte;
    unsigned char decryptedByte;
    FILE *inFile;
    FILE *outFile;

    inFile = fopen(inputFilename, "rb");
    outFile = fopen(outputFilename, "wb");

    if (inFile == NULL || outFile == NULL)
    {
        printf("Error in opening file");
        return 1;
    }

    printf("\nFile Opened, Decrypting");

    while ((inputByte = fgetc(inFile)) != EOF)
    {
        decryptedByte = inputByte ^ hash[hashIndex++ % SHA256_DIGEST_LENGTH];
        fputc(decryptedByte, outFile);
    }

    printf("End of File");

    fclose(inFile);
    fclose(outFile);

    return 0;
}