#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

int main() {
    char encFile[200], newencFile[200], decFile[200], newdecFile[200], passphrase[200];
    int choice;

    // Ask the user for the action they want to perform
    printf("Enter 1 to Encrypt / 2 to Decrypt: ");
    scanf("%d", &choice);

    // Ask for the passphrase to generate the hash
    printf("Enter the passphrase: ");
    scanf("%s", passphrase);

    switch (choice) {
        case 1:
            // Ask for filenames and call the Encrypt function with the passphrase
            // ...
            Encrypt(encFile, newencFile, passphrase);
            break;
        case 2:
            // Ask for filenames and call the Decrypt function with the passphrase
            // ...
            Decrypt(decFile, newdecFile, passphrase);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    return 0;
}


void createSHA256Hash(char *passphrase, char *hash);
int Encrypt(char *FILENAME, char *NEW_FILENAME, char *key);
int Decrypt(char *FILENAME, char *NEW_FILENAME, char *key);

#endif