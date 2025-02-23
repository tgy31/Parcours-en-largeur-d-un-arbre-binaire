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
    int result = 1;
    for (int i = 0; i < h + 1; i++) {
        result *= 2;
    }
    return result;
}

void insere_en_tete(Liste * l, Cellule * c) {
    if(*l == NULL) {
        *l = c;
    } else {
        c->suivant = *l;
        *l = c;
    }
} 

Cellule * extrait_tete(Liste * l) {
    if(*l == NULL) {
        return NULL;
    }
    Cellule * c = *l;
    *l = c->suivant;
    c->suivant = NULL;
    return c;
}


void affiche_liste_renversee(Liste lst) {
    if(lst == NULL) {
        return;
    }
    affiche_liste_renversee(lst->suivant);
    printf("%d ", lst->noeud->valeur);
}

File initialisation(void) {
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
    if(a == NULL) {
        return;
    }
    liberer_arbre(a->fg);
    liberer_arbre(a->fd);
    free(a);
}

int est_vide(File f) {
    if(f == NULL) {
        return 1;
    }
    return f->taille == 0;
}

int enfiler(File f, Noeud * n) {
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
    if(a == NULL){
        printf("0");
        return;
    }
    printf("%d ", a->valeur);
    affiche_arbre(a->fg);
    affiche_arbre(a->fd);
}


int construit_filiforme_aleatoire(int h, Arbre *a, int graine) {
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

