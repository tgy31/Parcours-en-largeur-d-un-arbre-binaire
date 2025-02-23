#include "DM_1_-_Parcours.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

int main() {
    Arbre a = NULL;
    Liste lst = NULL;
    int nb_visite = 0;
    int hauteur_complet = 25;   // Hauteur 5 -> 63 nœuds
    int nb_noeuds_filiforme = puissance_deux(hauteur_complet) - 1; // 63 nœuds pour l'arbre filiforme
    int graine = time(NULL);

    // Mesurer le temps pour construire un arbre complet
    clock_t debut = clock();
    construit_complet(hauteur_complet, &a);
    clock_t fin = clock();
    double temps_construit_complet = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour construire un arbre complet de hauteur %d: %f secondes\n", hauteur_complet, temps_construit_complet);

    // Parcours en largeur sur l'arbre complet
    debut = clock();
    parcours_largeur(a, &lst);
    fin = clock();
    double temps_parcours_complet = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour parcourir un arbre complet de hauteur %d avec parcours_largeur: %f secondes\n", hauteur_complet, temps_parcours_complet);

    lst = NULL;
    debut = clock();
    parcours_largeur_V2(a, &lst, &nb_visite);
    fin = clock();
    double temps_parcours_complet_V2 = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour parcourir un arbre complet de hauteur %d avec parcours_largeur_V2: %f secondes\n", hauteur_complet, temps_parcours_complet_V2);

    liberer_arbre(a);
    lst = NULL;

    // Mesurer le temps pour construire un arbre filiforme de 63 nœuds
    debut = clock();
    construit_filiforme_aleatoire(nb_noeuds_filiforme, &a, graine);
    fin = clock();
    double temps_construit_filiforme = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour construire un arbre filiforme de %d nœuds: %f secondes\n", nb_noeuds_filiforme, temps_construit_filiforme);

    // Parcours en largeur sur l'arbre filiforme
    debut = clock();
    parcours_largeur(a, &lst);
    fin = clock();
    double temps_parcours_filiforme = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour parcourir un arbre filiforme de %d nœuds avec parcours_largeur: %f secondes\n", nb_noeuds_filiforme, temps_parcours_filiforme);

    lst = NULL;
    debut = clock();
    parcours_largeur_V2(a, &lst, &nb_visite);
    fin = clock();
    double temps_parcours_filiforme_V2 = (double)(fin - debut) / CLOCKS_PER_SEC;
    printf("Temps pour parcourir un arbre filiforme de %d nœuds avec parcours_largeur_V2: %f secondes\n", nb_noeuds_filiforme, temps_parcours_filiforme_V2);

    liberer_arbre(a);
    lst = NULL;

    return 0;
}
