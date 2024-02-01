#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

typedef struct Block
{
    int index;
    char timestamp[64];
    char data[256];
    char hash[64];
    char previousHash[64];
} Block;

void calculateHash(Block *block, char *hash);
Block *createBlock(int index, char *timestamp, char *data, char *previousHash);

#endif