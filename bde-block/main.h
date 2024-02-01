#ifndef MAIN_H
#define MAIN_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct BaseDeDonnees
{
    char Token[32];
    int ID;
} BaD;

void nouvelleEntree(BaD *nouveau);
void recherche(BaD *db, int taille);
void liste(BaD *db, int taille);
int main(int argc, char **argv);

#endif