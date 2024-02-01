#include "main.h"

// Modified encryption function using XOR
int Encrypt(char *FILENAME, char *NEW_FILENAME, char *key) {
    // ... (initial setup and file opening code remains the same)

    unsigned char hash[SHA256_DIGEST_LENGTH];
    createSHA256Hash(key, hash); // Generate the hash from the passphrase

    int hashIndex = 0;
    while (!feof(inFile)) {
        Byte = fgetc(inFile);
        if(feof(inFile)) break; // Prevents reading past the end of the file
        newByte = Byte ^ hash[hashIndex++ % SHA256_DIGEST_LENGTH]; // XOR with hash key
        fputc(newByte, outFile);
    }

    // ... (file closing code remains the same)
}

// Modified decryption function using XOR
int Decrypt(char *FILENAME, char *NEW_FILENAME, char *key) {
    // ... (initial setup and file opening code remains the same)

    unsigned char hash[SHA256_DIGEST_LENGTH];
    createSHA256Hash(key, hash); // Generate the hash from the passphrase

    int hashIndex = 0;
    while (!feof(inFile)) {
        Byte = fgetc(inFile);
        if(feof(inFile)) break; // Prevents reading past the end of the file
        newByte = Byte ^ hash[hashIndex++ % SHA256_DIGEST_LENGTH]; // XOR with hash key
        fputc(newByte, outFile);
    }

    // ... (file closing code remains the same)
}