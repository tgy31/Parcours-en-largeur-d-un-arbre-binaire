#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
        return 0;
    }

    Cellule * c = extrait_tete(&(f->debut));
    if(c == NULL) {
        return 0;
    }
    *sortant = c->noeud;
    free(c);
    f->taille--;
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
    assert(h >= 0); // Vérifier que la hauteur est non négative

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

    while (val <= (1 << (h + 1)) - 1) {
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
    //affiche dans un ordre prefixe et affiche 0 pour un fils vide
    if(a == NULL){
        printf("0 ");
        return;
    }
    printf("%d ", a->valeur);
    affiche_arbre(a->fg);
    affiche_arbre(a->fd);
}

int main(){
    Arbre a = NULL;
    construit_complet(3, &a);
    affiche_arbre(a);
    return 0;
}