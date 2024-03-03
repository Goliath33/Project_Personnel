#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    int index;
    char timestamp[20];
    char data[1024];
    char previousHash[SHA256_DIGEST_LENGTH * 2 + 1];
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
} Block;

//Prototypes
int main();
void fatal(const char *message);
void calculateHash(char *data, size_t length, unsigned char *hash);
void calculateHash(Block *block);
void printBlock(Block *block);
void printBlocks(Block **blocks, int length);
void printHash(char *hash);
Block **createChain(int length);
Block *createGenesisBlock();
Block *createBlock(int index, char *timestamp, char *data, char *previousHash);
Block *addBlock(Block *blockchain, int index, char *data);

#endif