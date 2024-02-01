#include "main.h"

void calculateHash(Block *block, char *hash)
{
    char data[1024];
    sprintf(data, "%d%s%s%s", block->index, block->timestamp, block->data, block->previousHash);

    unsigned char temp[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)data, strlen(data), temp);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hash + (i * 2), "%02x", temp[i]);
    }
}


Block *createBlock(int index, char *timestamp, char *data, char *previousHash)
{
    Block *block = (Block *)malloc(sizeof(Block));
    block->index = index;
    strcpy(block->timestamp, timestamp);
    strcpy(block->data, data);
    strcpy(block->previousHash, previousHash);
    calculateHash(block, block->hash);

    return block;
}

int main()
{
    Block *genesisBlock = createBlock(0, "01/01/2024", "Genesis Block", "0");
    printf("Index: %d\n", genesisBlock->index);
    printf("Timestamp: %s\n", genesisBlock->timestamp);
    printf("Data: %s\n", genesisBlock->data);
    printf("Hash: %s\n", genesisBlock->hash);
    printf("Previous Hash: %s\n", genesisBlock->previousHash);

    return 0;
}