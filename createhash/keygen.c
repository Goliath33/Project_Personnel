#include "main.h"

// Function to create a SHA256 hash from a passphrase
void createSHA256Hash(char *passphrase, char *hash) {
    unsigned char temp[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)passphrase, strlen(passphrase), temp);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", temp[i]);
    }
}

