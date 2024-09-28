#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Bplus/Bplus.h"


int comp_int(void* a, void*b) {
    int* ia = (int*) a;
    int* ib = (int*) b;

    if (*ia == *ib)
        return 1;
    if (*ia < *ib)
        return 0;
    if (*ia > *ib)
        return 2;
}

void print_int(void* a) {
    int *ai = (int*) a;

    printf("%d ", *ai);
}

int main() {
    printf("alocação:-------\n\n");
    Bplus *tree = Bplus_alloc(comp_int);

    printf("inserção:-------\n\n");
    int *v;
    for (int i = 10; i < 140; i +=10)
    {
        v = malloc(sizeof(int));
        *v = i;
        Bplus_insert(tree, v);
    }
    
    int b = 10;
    printf("busca:---------\n\n");
    node *find = Bplus_find(tree->root, &b);

    print_node(find, print_int);

    printf("\n");

    printf("busca em intervalo:\n\n");
    b = 20;
    int c = 80;
    node **nos = Bplus_findInterval(tree->root, &b, &c);

    for (int i = 0; nos[i] != NULL; i++)
    {
        print_node(nos[i], print_int);
    }

    printf("Árvore B+:---------\n\n");
    Bplus_print(tree, print_int);
    
    printf("remoção:---------\n\n");
    b = 40;
    Bplus_remove(tree, &b);

    printf("\n");

    Bplus_print(tree, print_int);

    printf("liberando memória:---------\n\n");
    Bplus_free(tree);
}