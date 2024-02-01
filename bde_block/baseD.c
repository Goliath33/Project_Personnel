#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nouvelleEntree(BaD *nouveau)
{
    printf("Nouvelle entrée dans la base de données:\n");
    printf("Entrez le Token: ");
    fgets(nouveau->Token, sizeof(nouveau->Token), stdin);
    nouveau->Token[strcspn(nouveau->Token, "\n")] = 0; // Enlève le '\n'
    printf("Entrez l'ID: ");
    char line[256];
    if (fgets(line, sizeof(line), stdin))
    {
        sscanf(line, "%d", &(nouveau->ID));
    }
}

void recherche(BaD *db, int taille)
{
    char search[32];
    printf("Entrez le Token à chercher: ");
    scanf("%s", search);
    int index = 0;
    while (index < taille && strcmp(db[index].Token, search) != 0)
        index++;
    if (index < taille)
        printf("L'ID de %s est %d\n", search, db[index].ID);
    else
        printf("Le Token '%s' n'existe pas dans la base de données.\n", search);
}

void liste(BaD *db, int taille)
{
    printf("\nListe:\n");
    for (int i = 0; i < taille; i++)
        printf("%d) %s %d\n", i + 1, db[i].Token, db[i].ID);
    printf("\n");
}

int main(int argc, char **argv)
{
    BaD db[100];
    int taille = 0;
    char choix;

    do
    {
        printf("Menu:\n");
        printf("1) Nouvelle entrée\n");
        printf("2) Rechercher\n");
        printf("3) Liste\n");
        printf("4) Quitter\n");
        printf("Votre choix: ");
        scanf(" %c", &choix);
        switch (choix)
        {
        case '1':
            if (taille < 100)
            {
                nouvelleEntree(&db[taille]);
                taille++;
            }
            else
            {
                printf("Database full!");
            }
            break;
        case '2':
            recherche(db, taille);
            break;
        case '3':
            liste(db, taille);
            break;
        case '4':
            printf("Au revoir!\n");
            break;
        default:
            printf("Choix invalide\n");
        }
    } while (choix != '4');
    return 0;
}