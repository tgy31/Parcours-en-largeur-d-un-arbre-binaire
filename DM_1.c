#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, * Arbre;

typedef struct cell {
    Noeud * noeud;
    struct cell * suivant;
} Cellule, * Liste;

typedef struct {
    Liste debut;
    Liste fin;
    int taille;
} Queue, * File;

Cellule * alloue_cellule(Noeud * n) {
    /* Alloue une cellule et l'initialise avec un noeud donné
    Param n : Pointeur vers le noeud à stocker dans la cellule
    Return : Pointeur vers la cellule allouée, initialisée avec le noeud */
    Cellule * c = (Cellule *) malloc(sizeof(Cellule));
    if(c == NULL){
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    c->noeud = n;
    c->suivant = NULL;
    return c;
}

int puissance_deux(int h) {
    /* Calcule la puissance de deux pour un entier donné
    Param h : Exposant de la puissance de deux
    Return : 2^(h) */
    assert(h >= 0);
    int result = 1;
    for (int i = 0; i < h + 1; i++) {
        result *= 2;
    }
    return result;
}

void insere_en_tete(Liste * l, Cellule * c) {
     /* Insère une cellule en tête d'une liste chaînée
    Param l : Pointeur vers la tête de la liste
    Param c : Pointeur vers la cellule à insérer
    Return : Aucun */
    if(*l == NULL) {
        *l = c;
    } else {
        c->suivant = *l;
        *l = c;
    }
} 

Cellule * extrait_tete(Liste * l) {
    /* Extrait la première cellule d'une liste chaînée
    Param l : Pointeur vers la tête de la liste
    Return : Pointeur vers la cellule extraite, ou NULL si la liste est vide */
    if(*l == NULL) {
        return NULL;
    }
    Cellule * c = *l;
    *l = c->suivant;
    c->suivant = NULL;
    return c;
}


void affiche_liste_renversee(Liste lst) {
    /* Affiche les éléments d'une liste chaînée en ordre inverse
    Param lst : Liste à afficher (pointeur vers la tête)
    Return : Rien */
    if(lst == NULL) {
        return;
    }
    affiche_liste_renversee(lst->suivant);
    printf("%d ", lst->noeud->valeur);
}

File initialisation(void) {
    /* Initialise une file vide
    Param : Aucun
    Return : Une file vide allouée */
    File f = (File) malloc(sizeof(Queue));
    if(f == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    f->debut = NULL;
    f->fin = NULL;
    f->taille = 0;
    return f;
}

void liberer_arbre(Arbre a) {
    /* Libère récursivement l'arbre binaire en supprimant les noeuds
    Param a : Pointeur vers la racine de l'arbre à libérer
    Return : Rien
    Effet de bord : Libère toute la mémoire allouée pour l'arbre */
    if(a == NULL) {
        return;
    }
    liberer_arbre(a->fg);
    liberer_arbre(a->fd);
    free(a);
}

int est_vide(File f) {
    /* Vérifie si la file est vide
    Param f : Pointeur vers la file à vérifier
    Return : 1 si la file est vide, 0 sinon */
    if(f == NULL) {
        return 1;
    }
    return f->taille == 0;
}

int enfiler(File f, Noeud * n) {
    /* Ajoute un noeud à la fin de la file
    Param f : Pointeur vers la file où ajouter l'élément
    Param n : Pointeur vers le noeud à ajouter à la file
    Return : 1 si l'ajout a réussi, 0 sinon
    Effet de bord : Ajoute un élément à la file et met à jour la taille */
    Cellule * c = alloue_cellule(n);
    if(c == NULL) {
        return 0;
    }
    if(f->taille == 0) {
        f->debut = c;
        f->fin = c;
    } else {
        f->fin->suivant = c;
        f->fin = c;
    }
    f->taille++;
    return 1;
}

int defiler(File f, Noeud ** sortant) {
    /* Retire le premier noeud de la file et le retourne
    Param f : Pointeur vers la file à traiter
    Param sortant : Pointeur vers le noeud à récupérer
    Return : 1 si un élément a été défini avec succès, 0 si la file était vide
    Effet de bord : La file est mise à jour après avoir retiré le premier élément */
    if(est_vide(f)) {
        *sortant = NULL;
        return 0;
    }
    Cellule * c = extrait_tete(&(f->debut));
    *sortant = c->noeud;
    free(c);
    f->taille--;
    if(f->taille == 0) {
        f->fin = NULL; 
    }
    return 1;
}


Noeud * alloue_noeud(int val, Arbre fg, Arbre fd) {
    /* Alloue un noeud pour un arbre binaire
    Param val : La valeur à attribuer au noeud
    Param fg : Le sous-arbre gauche
    Param fd : Le sous-arbre droit
    Return : Un pointeur vers le noeud alloué */
    Noeud * n = (Noeud *) malloc(sizeof(Noeud));
    if(n == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    n->valeur = val;
    n->fg = fg;
    n->fd = fd;
    return n;
}

int construit_complet(int h, Arbre *a) {
    /* Construit un arbre binaire complet de hauteur h
    Param h : La hauteur de l'arbre
    Param a : Pointeur vers la racine de l'arbre à construire
    Return : 1 si la construction a réussi, 0 si une erreur s'est produite */
    if (h == 0) {
        *a = NULL;
        return 1;
    }

    File f = initialisation();
    if (f == NULL) {
        return 0;
    }

    int val = 1;
    *a = alloue_noeud(val++, NULL, NULL);
    if (*a == NULL) {
        free(f);
        return 0;
    }

    if (!enfiler(f, *a)) {
        liberer_arbre(*a);
        free(f);
        return 0;
    }

    while (val <= puissance_deux(h) - 1) {
        Noeud *courant;
        if (!defiler(f, &courant)) {
            liberer_arbre(*a);
            free(f);
            return 0;
        }

        courant->fg = alloue_noeud(val++, NULL, NULL);
        if (courant->fg == NULL) {
            liberer_arbre(*a);
            free(f);
            return 0;
        }
        if (!enfiler(f, courant->fg)) {
            liberer_arbre(*a);
            free(f);
            return 0;
        }

        if (val <= (1 << (h + 1)) - 1) {
            courant->fd = alloue_noeud(val++, NULL, NULL);
            if (courant->fd == NULL) {
                liberer_arbre(*a);
                free(f);
                return 0;
            }
            if (!enfiler(f, courant->fd)) {
                liberer_arbre(*a);
                free(f);
                return 0;
            }
        }
    }

    free(f);
    return 1;
}

void affiche_arbre(Arbre a){
     /* Affiche l'arbre binaire de manière préfixe
    Param a : Pointeur vers la racine de l'arbre à afficher
    Effet de bord : Affiche les valeurs des noeuds de l'arbre à l'écran sous la forme "valeur1 valeur2 ... valeurN"
    Si l'arbre est vide, affiche "0" */
    if(a == NULL){
        printf("0");
        return;
    }
    printf("%d ", a->valeur);
    affiche_arbre(a->fg);
    affiche_arbre(a->fd);
}


int construit_filiforme_aleatoire(int h, Arbre *a, int graine) {
    /* Construit un arbre filiforme de manière aléatoire
    Param h : La hauteur de l'arbre
    Param a : Pointeur vers la racine de l'arbre à construire
    Param graine : La graine pour la génération de nombres aléatoires
    Return : 1 si la construction a réussi, 0 en cas d'erreur
    Effet de bord : Alloue de la mémoire pour l'arbre et ses noeuds */
    if (h < 0) {
        return 0; 
    }
    srand(graine);
    *a = alloue_noeud(1, NULL, NULL);
    if (*a == NULL) {
        return 0;
    }
    Arbre courant = *a;
    for (int i = 2; i <= h + 1; ++i) {
        if (rand() % 2 == 0) {
            courant->fg = alloue_noeud(i, NULL, NULL);
            courant = courant->fg;
        } else {
            courant->fd = alloue_noeud(i, NULL, NULL);  
            courant = courant->fd; 
        }
    }
    return 1; 
}


int insere_niveau(Arbre a, int niv, Liste * lst){
    /* Insère les noeuds d'un arbre à un niveau donné dans une liste
    Param a : Pointeur vers l'arbre (pouvant être NULL)
    Param niv : Le niveau auquel on souhaite insérer les noeuds dans la liste
    Param lst : Liste où seront insérés les noeuds du niveau donné
    Return : 1 si l'insertion est réussie, 0 en cas d'erreur
    Effet de bord : Insère les noeuds trouvés au niveau 'niv' de l'arbre dans la liste 'lst' */
    if (a == NULL) {
        *lst = NULL;
        return 1;
    }
    if (niv == 0) {
        Cellule * c = alloue_cellule(a);
        if (c == NULL) {
            return 0;
        }
        insere_en_tete(lst, c);
        return 1;
    }
    int fg = insere_niveau(a->fg, niv - 1, lst);
    int fd = insere_niveau(a->fd, niv - 1, lst);
    return fg || fd;
}

int hauteur_arbre(Arbre a) {
    /* Calcule la hauteur de l'arbre binaire
    Param a : Pointeur vers la racine de l'arbre
    Return : La hauteur de l'arbre (0 si l'arbre est vide) */
    if (a == NULL) {
        return 0;
    }
    int hg = hauteur_arbre(a->fg);
    int hd = hauteur_arbre(a->fd);
    if (hg > hd) {
        return hg + 1;
    }
    return hd + 1;
}

int parcours_largeur_naif(Arbre a, Liste * lst){
    /* Parcourt l'arbre en largeur (par niveau) et insère les noeuds dans la liste
    Param a : Pointeur vers la racine de l'arbre
    Param lst : Liste dans laquelle seront insérés les noeuds lors du parcours
    Return : 1 si le parcours en largeur a réussi, 0 en cas d'erreur */
    if (a == NULL) {
        return 0;
    }
    int hauteur = hauteur_arbre(a);
    for (int i = 0; i < hauteur; ++i) {
        if (!insere_niveau(a, i, lst)) {
            return 0;
        }
    }
    return 1;
}


int parcours_largeur(Arbre a, Liste * lst) {
    /* Effectue un parcours en largeur de l'arbre et insère les noeuds dans la liste
    Param a : Pointeur vers la racine de l'arbre
    Param lst : Liste dans laquelle seront insérés les noeuds lors du parcours
    Return : 1 si le parcours en largeur a réussi, 0 en cas d'erreur
    Effet de bord : Remplie la liste 'lst' avec les noeuds de l'arbre et met à jour 'nb_visite' */
    if (a == NULL) {
        return 0;
    }
    File f = initialisation();
    if (f == NULL) {
        return 0;
    }
    if (!enfiler(f, a)) {
        free(f);
        return 0;
    }
    while (!est_vide(f)) {
        Noeud * courant;
        if (!defiler(f, &courant)) {
            free(f);
            return 0;
        }
        if (courant != NULL) {
            if (courant->fg != NULL) {
                if (!enfiler(f, courant->fg)) {
                    free(f);
                    return 0;
                }
            }
            if (courant->fd != NULL) {
                if (!enfiler(f, courant->fd)) {
                    free(f);
                    return 0;
                }
            }
            Cellule * c = alloue_cellule(courant);
            if (c == NULL) {
                free(f);
                return 0;
            }
            insere_en_tete(lst, c);
        }
    }
    free(f);
    return 1;
}

int parcours_largeur_V2(Arbre a, Liste * lst, int * nb_visite) {
    /* Effectue un parcours en largeur de l'arbre et insère les noeuds dans la liste tout en comptant les noeuds visités
    Param a : Pointeur vers la racine de l'arbre
    Param lst : Liste dans laquelle seront insérés les noeuds lors du parcours
    Param nb_visite : Pointeur vers un entier qui sera mis à jour avec le nombre de noeuds visités
    Return : 1 si le parcours en largeur a réussi, 0 en cas d'erreur 
    Effet de bord : Remplie la liste 'lst' avec les noeuds de l'arbre et met à jour 'nb_visite' */
    if (a == NULL) {
        return 0;
    }
    *nb_visite = 0;
    File f = initialisation();
    if (f == NULL) {
        return 0;
    }
    if (!enfiler(f, a)) {
        free(f);
        return 0;
    }
    while (!est_vide(f)) {
        Noeud * courant;
        if (!defiler(f, &courant)) {
            free(f);
            return 0;
        }
        if (courant != NULL) {
            (*nb_visite)++;
            if (courant->fg != NULL) {
                if (!enfiler(f, courant->fg)) {
                    free(f);
                    return 0;
                }
            }
            if (courant->fd != NULL) {
                if (!enfiler(f, courant->fd)) {
                    free(f);
                    return 0;
                }
            }
            Cellule * c = alloue_cellule(courant);
            if (c == NULL) {
                free(f);
                return 0;
            }
            insere_en_tete(lst, c);
        }
    }
    free(f);
    return 1;
}

int parcours_largeur_naif_V2(Arbre a, Liste * lst, int * nb_visite) {
     /* Effectue un parcours en largeur naïf de l'arbre et insère les noeuds dans la liste tout en comptant les noeuds visités
    Param a : Pointeur vers la racine de l'arbre
    Param lst : Liste dans laquelle seront insérés les noeuds lors du parcours
    Param nb_visite : Pointeur vers un entier qui sera mis à jour avec le nombre de noeuds visités
    Return : 1 si le parcours en largeur a réussi, 0 en cas d'erreur
    Effet de bord : Remplie la liste 'lst' avec les noeuds de l'arbre et met à jour 'nb_visite' */
    if (a == NULL) {
        return 0;
    }
    *nb_visite = 0;
    int hauteur = hauteur_arbre(a);
    for (int i = 0; i < hauteur; ++i) {
        int nb_visite_niveau = 0;
        if (!insere_niveau(a, i, lst)) {
            return 0;
        }
        Cellule *temp = *lst;
        while (temp != NULL) {
            nb_visite_niveau++;
            temp = temp->suivant;
        }
        *nb_visite += nb_visite_niveau;
    }
    return 1;
}
