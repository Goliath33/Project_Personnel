#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generetoken()
{
    srand(time(0));             // Utilisez l'heure actuelle comme graine pour le générateur de nombres aléatoires
    int random_number = rand(); // Génère un nombre aléatoire

    char token[100];
    sprintf(token, "%d", random_number); // Convertit le nombre aléatoire en une chaîne de caractères

    printf("Token: %s\n", token);
}

int main() {
    generetoken();
    return 0;
}