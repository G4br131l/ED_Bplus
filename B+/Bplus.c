#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Bplus.h"

Bplus *Bplus_alloc() {
    Bplus *tree = malloc(sizeof(Bplus));
    if (!tree) {
        printf("erro alloc tree");
        exit(1);
    }
    tree->root = NULL;
    tree->last_list = NULL;

    return tree;
}

node *node_alloc(bool leaf) {
    node *no = malloc(sizeof(node));

    no->length = 0;

    for (int i = 0; i < 4; i++)
    {
        no->p[i] = NULL;
    }
    
    no->befor = no->left = no->rigth = NULL;
    no->leaf = leaf;

    return no;
}

void free_node(node *node) {
    if (node->leaf)
    {
        free(node);
    }
    
    for (int i = 0; i < 4; i++)
    {
        free_node(node->p[i]);
    }
    
    free(node);
}

void Bplus_free(Bplus *tree) {
    if (!tree) {
        return;
    }
    
    free_node(tree->root);
    free(tree);
}
/* 
* @return retorna o valor do indice em que o value deve ficar
 */
int search_in_node(node *no, int value) {
    for (int i = 0; i < no->length; i++) {
        if (no->index[i] > value) {
            return i;
        }
    }
    return no->length;
}
/* 
* apartir do inicio a lista do no sera passada um indice para frente.
* @param node é o no em questão, em que a lista vai ser auterada
* @param inicio desse ponto para frente o valor sera alterado
 */
void shift_node(node *no, int inicio) {
    for (int i = no->length; i > inicio; i--) {
        no->index[i] = no->index[i - 1];
    }
}

void insert_in_leaf(node *no, int value) {
    if (no->length < ORDER) {
        int index = search_in_node(no, value);
        if (index != no->length)
            shift_node(no, index);
        no->index[index] = value;
        no->length++;
        print_node(no);
        return NULL;
    }

    node *noL = node_alloc(true);
    node *noR = node_alloc(true);

    int midi = ORDER/2;

    for (int i = 0; i < midi; i++) {
        noL->index[noL->length++] = no->index[i];
    }
    noL->befor = no;

    for (int i = midi; i < ORDER; i++) {
        noR->index[noR->length++] = no->index[i];
    }
    noR->befor = no;

    if (no->left != NULL) {
        noL->left = no->left;
        no->left->rigth = noL;
    }
    if (no->rigth != NULL) {
        noR->rigth = no->rigth;
        no->rigth->left = noR;
    }
    
    noL->rigth = noR;
    noR->left = noL;

    no->index[0] = no->index[midi];
    no->length = 1;
    no->leaf = false;
    no->p[0] = noL;
    no->p[1] = noR;

    if (value < no->index[0])
        insert_in_leaf(noL, value);
    else 
        insert_in_leaf(noR, value);

    if (no->befor->length)
    {
        /* code */
    }
    

    print_node(no);
    print_node(noL);
    print_node(noR);

    return noR;
}

void node_insert(node *no, int value) {
 
    if (!no->leaf) {
        int cont = 0;
        for (int i = 0; i < no->length; i++) {
            if (no->index[i] <= value) {
                cont++;
            }
        }
        printf("%d---\n", cont);
        node_insert(no->p[cont], value);
        return;
    }
    
    insert_in_leaf(no, value);

}

void Bplus_insert(Bplus *tree, int value) {
    if (!tree || !value)
    {
        printf("erro inert");
        return;
    }
    
    if (tree->root == NULL)
    {
        node *no = node_alloc(true);

        no->leaf = true;

        no->index[no->length++] = value;

        tree->root = no;
        return;
    }
    
    if (tree->root->leaf) {
        insert_in_leaf(tree->root, value);
        return;
    }

    node_insert(tree->root, value);
}



// Função para imprimir a árvore B+ (para debug)
void print_tree(node *n) {
    if (n != NULL) {
        for (int i = 0; i < n->length; i++) {
            printf("%d ", n->index[i]);
        }
        printf("| ");

        for (int i = 0; i < 4; i++)
        {
            print_tree(n->p[i]);
        }
    }
}

void print_node(node* no) {
    printf("%ld\n", no);
    printf("index: ");
    for (int i = 0; i < no->length; i++)
    {
        printf("%d ", no->index[i]);
    }
    printf("\n");
    printf("legth: %d\n", no->length);
    printf("leaf: %d\n", no->leaf);

    printf("before: ");
    if (no->befor)
    {
        for (int i = 0; i < no->befor->length; i++)
        {
            printf("%d", no->befor->index[i]);
        }
    }
    printf("\n");
    
    if (!no->leaf)
    {
        printf("\np:\n");
        for (int i = 0; i < 4; i++)
        {
            printf("%d: ", i);
            if (no->p[i] != NULL)
            {
                for (int j = 0; j < no->p[i]->length; j++)
                {
                    printf("%d ", no->p[i]->index[j]);
                }
            }
            printf("\n");
        }
    } else {
        printf("left: ");
        if (no->left != NULL)
        for (int i = 0; i < no->left->length; i++)
        {
            printf("%d ", no->left->index[i]);
        }
        printf("\n");
        
        printf("rigth: ");
        if (no->rigth != NULL)
        for (int i = 0; i < no->rigth->length; i++)
        {
            printf("%d ", no->rigth->index[i]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    Bplus *tree = Bplus_alloc();

    Bplus_insert(tree, 50);
    Bplus_insert(tree, 60);
    Bplus_insert(tree, 40);
    Bplus_insert(tree, 70);
    Bplus_insert(tree, 30);
    Bplus_insert(tree, 80);
    Bplus_insert(tree, 55);
    Bplus_insert(tree, 20);
    Bplus_insert(tree, 10);
    Bplus_insert(tree, 90);
    Bplus_insert(tree, 100);
    Bplus_insert(tree, 110);
    Bplus_insert(tree, 120);
    

    /* insert(tree, 10);
    insert(tree, 20);
    insert(tree, 5);
    insert(tree, 6);
    insert(tree, 12);
    insert(tree, 30);
    insert(tree, 7);
    insert(tree, 17);
    printf("oi");

    printf("Árvore B+:\n");
    print_tree(tree->root); */

    return 0;
}