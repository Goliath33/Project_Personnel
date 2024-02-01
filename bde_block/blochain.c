#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Transaction
{
    char sender[32];
    char receiver[32];
    float amount;
} Transaction;

typedef struct Block
{
    int ID;
    Transaction *transactions; // Un tableau de transactions
    int numTransactions;       // Le nombre de transactions dans le bloc
    struct Block *next;
} Block;

Block *addTransaction(Block *block, char *sender, char *receiver, float amount)
{
    Transaction newTransaction;
    strcpy(newTransaction.sender, sender);
    strcpy(newTransaction.receiver, receiver);
    newTransaction.amount = amount;

    block->transactions[block->numTransactions] = newTransaction;
    block->numTransactions++;

    return block;
}

Block *createBlock(int ID, char *Token)
{
    Block *newBlock = malloc(sizeof(Block));
    newBlock->ID = ID;
    strcpy(newBlock -> Token, Token);
    newBlock->next = NULL;
    return newBlock;
}

Block *addBlock(Block *blockchain, int ID, char *Token)
{
    Block *newBlock = createBlock(ID, Token);
    Block *currentBlock = blockchain;
    while (currentBlock->next != NULL)
    {
        currentBlock = currentBlock->next;
    }
    currentBlock->next = newBlock;
    return blockchain;
}

void printBlockchain(Block *blockchain)
{
    Block *currentBlock = blockchain;
    while (currentBlock != NULL)
    {
        printf("ID: %d, Token: %s\n", currentBlock->ID, currentBlock -> Token);
        currentBlock = currentBlock->next;
    }
}

int main()
{
    Block *blockchain = createBlock(0, "Genesis Block");
    blockchain = addBlock(blockchain, 1, "Block 1");
    blockchain = addBlock(blockchain, 2, "Block 2");
    printBlockchain(blockchain);
    return 0;
}