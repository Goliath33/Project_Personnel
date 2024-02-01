#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

int Encrypt(char *FILENAME, char *NEW_FILENAME);
int Decrypt(char *FILENAME, char *NEW_FILENAME);

#endif