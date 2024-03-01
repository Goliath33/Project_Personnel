#include "main.h"


void fatal(const char *message)
{
    char error_message[1024];
    snprintf(error_message, sizeof(error_message), "%s: %s", message, strerror(errno));
    perror(error_message);
    exit(1);
}

void calculateHash(char *data, size_t length, unsigned char *hash)
{
    SHA256((unsigned char *)data, length, hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        snprintf(hash + (i * 2), sizeof(hash) - (i * 2), "%02x", hash[i]);
    }
    hash[SHA256_DIGEST_LENGTH * 2] = '\0';
    return;
}

void calculateHash(Block *block)
{
    char data[1024];
    int i;

    snprintf(data, sizeof(data), "%d%s%s%s", block->index, block->timestamp, block->data, block->previousHash);

    unsigned char temp[SHA256_DIGEST_LENGTH];
    calculateHash(data, strlen(data), temp);

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        snprintf(block->hash + (i * 2), sizeof(block->hash) - (i * 2), "%02x", temp[i]);
    }
}

void printHash(char *hash)
{
    int i;

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

Block *addBlock(Block *blockchain, int index, char *data)
{
    Block *newBlock = createBlock(index, "01/01/2024", data, blockchain->hash);
    return newBlock;
}

Block *createBlock(int index, char *timestamp, char *data, char *previousHash)
{
    Block *block = malloc(sizeof(Block));
    block->index = index;
    strcpy(block->timestamp, timestamp);
    strcpy(block->data, data);
    strcpy(block->previousHash, previousHash);
    calculateHash(block);

    return block;
}

Block *createGenesisBlock()
{
    return createBlock(0, "01/01/2024", "Genesis Block", "0");
}

Block **createChain(int length)
{
    Block **blocks = malloc(sizeof(Block *) * length);
    blocks[0] = createGenesisBlock();
    for (int i = 1; i < length; i++)
    {
        blocks[i] = createBlock(i, "01/01/2024", "Data", blocks[i - 1]->hash);
    }
    return blocks;
}

void printBlock(Block *block)
{
    printf("Index: %d\n", block->index);
    printf("Timestamp: %s\n", block->timestamp);
    printf("Data: %s\n", block->data);
    printf("Hash: %s\n", block->hash);
    printf("Previous Hash: %s\n", block->previousHash);
}

void printBlocks(Block **blocks, int length)
{
    for (int i = 0; i < length; i++)
    {
        printBlock(blocks[i]);
        printf("\n");
        free(blocks[i]);
    }
    free(blocks);
}                                   


char Byte; // Variable to store the byte read from the file

int main()
{
    Block *genesisBlock = createBlock(0, "01/01/2024", "Genesis Block", "0");

    printf("Index: %d\n", genesisBlock->index);
    printf("Timestamp: %s\n", genesisBlock->timestamp);
    printf("Data: %s\n", genesisBlock->data);
    printf("Hash: %s\n", genesisBlock->hash);
    printf("Previous Hash: %s\n", genesisBlock->previousHash);

    free(genesisBlock);

    return 0;
}