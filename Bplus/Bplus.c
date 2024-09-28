#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Bplus.h"

Bplus *Bplus_alloc(int (*comp)(void*a, void*b)) {
    Bplus *tree = malloc(sizeof(Bplus));
    if (!tree) {
        printf("erro alloc tree");
        exit(1);
    }
    tree->root = NULL;
    tree->comp = comp;

    return tree;
}

node *node_alloc(bool leaf, int (*comp)(void*,void*)) {
    node *no = malloc(sizeof(node));

    no->length = 0;

    for (int i = 0; i < 4; i++)
    {
        no->p[i] = NULL;
    }
    
    no->befor = no->left = no->rigth = NULL;
    no->leaf = leaf;
    no->comp = comp;

    return no;
}

void free_node(node *node) {
    if (node->leaf)
    {
        free(node);
        return;
    }
    
    for (int i = 0; i < node->length+1; i++)
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
int search_in_node(node *no, void* value) {
    for (int i = 0; i < no->length; i++) {
        if (no->comp(no->index[i], value) == 2) {
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

    for (int i = no->length+1; i > inicio; i--) {
        no->index[i] = no->index[i - 1];
    }
}

void insert_in_node(node *no, node *before) {
    int index = search_in_node(before, no->index[0]);
    if (index != no->length)
        shift_node(before, index);
    
    before->index[index] = no->index[0];
    before->p[index] = no->p[0];
    before->p[index+1] = no->p[1];

    before->length++;
}

void insert_in_leaf(node *no, void* value) {
    if (no->length < ORDER) {
        
        int index = search_in_node(no, value);
        if (index != no->length)
            shift_node(no, index);
        no->index[index] = value;
        no->length++;
        return;
    }

    node *noL = node_alloc(true, no->comp);
    node *noR = node_alloc(true, no->comp);

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


    if (no->comp(value, no->index[0]) == 0)
        insert_in_leaf(noL, value);
    else 
        insert_in_leaf(noR, value);

    if (no->befor != NULL && no->befor->length < ORDER)
    {
        insert_in_node(no, no->befor);
    }
}

void node_insert(node *no, void* value) {
 
    if (!no->leaf) {
        int cont = 0;
        for (int i = 0; i < no->length; i++) {
            if (no->comp(no->index[i], value) != 2) {
                cont++;
            }
        }
        node_insert(no->p[cont], value);
        return;
    }
    
    insert_in_leaf(no, value);
}

void Bplus_insert(Bplus *tree, void* value) {
    if (!tree || !value)
    {
        printf("erro insert");
        return;
    }
    
    if (tree->root == NULL)
    {
        node *no = node_alloc(true, tree->comp);

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

void node_remove(node *no, void* value, bool no_com_valor, node* no_value) {
    if (!no->leaf) {
        int cont = 0;
        for (int i = 0; i < no->length; i++) {
            if (no->comp(no->index[i], value) != 2) {
                cont++;
            }
            if (no->comp(no->index[i], value) == 1 && !no_com_valor) {
                no_com_valor = true;
            }
        }
        node_remove(no->p[cont], value, no_com_valor, no);
        return;
    }

    int local = -1;
    for (int i = 0; i < no->length; i++)
    {
        if (no->comp(no->index[i], value) == 1) {
            local = i;
            no->length--;
            for (int i = local; i < no->length; i++)
            {
                no->index[i] = no->index[i+1];
            }
            break;
        }
    }
    
    if (no_com_valor) {
        for (int i = 0; i < no_value->length; i++)
        {
            if (no->comp(no_value->index[i], value) == 1)
            {
                no_value->index[i] = no->index[local];
                break;
            }
        }
    }
}

void Bplus_remove(Bplus *tree, void* value) {
    if (tree->root->leaf)
    {
        for (int i = 0; i < tree->root->length; i++)
        {
            if (tree->root->comp(tree->root->index[i], value) == 1)
            {
                tree->root->length--;
                for (int i = 0; i < tree->root->length; i++)
                {
                    tree->root->index[i] = tree->root->index[i+1];
                }
                break;
            }
        }
    }
    
    node_remove(tree->root, value, false, NULL);
}

// Função para imprimir a árvore B+ (para debug)
void print_tree(node *n, int profundidade, void (*print)(void*)) {
    if (n != NULL) {
        for (int j = 0; j < profundidade; j++) {
            printf("  ");
        }

        for (int i = 0; i < n->length; i++) {
            print(n->index[i]);
        }
        printf("\n");

        if (!n->leaf)
        {
            for (int i = 0; i < n->length+1; i++)
            {
                print_tree(n->p[i], profundidade + 1, print);
            }
        }
    }
}

void Bplus_print(Bplus *tree, void (*print)(void*)) {
    if (!tree)
    {
        printf("erro print");
        return;
    }
    
    print_tree(tree->root, 0, print);
}

void print_node(node* no, void(*print)(void*)) {
    printf("%ld\n", no);
    printf("index: ");
    for (int i = 0; i < no->length; i++)
    {
        print( no->index[i]);
    }
    printf("\n");
    printf("legth: %d\n", no->length);
    printf("leaf: %d\n", no->leaf);

    printf("before: ");
    if (no->befor)
    {
        for (int i = 0; i < no->befor->length; i++)
        {
            print(no->befor->index[i]);
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
                    print(no->p[i]->index[j]);
                }
            }
            printf("\n");
        }
    } else {
        printf("left: ");
        if (no->left != NULL)
        for (int i = 0; i < no->left->length; i++)
        {
            printf("%d ", *((int*) no->left->index[i]));
        }
        printf("\n");
        
        printf("rigth: ");
        if (no->rigth != NULL)
        for (int i = 0; i < no->rigth->length; i++)
        {
            print(no->rigth->index[i]);
        }
        printf("\n");
    }
    printf("\n");
}

node *Bplus_find(node *no, void* value) {
    if (!no) {
        printf("Árvore está vazia ou nó nulo.\n");
        return NULL;
    }

    if (no->leaf) {
        for (int i = 0; i < no->length; i++) {
            if (no->comp(no->index[i], value) == 1) {
                return no;
            }
        }
        return NULL;
    }

    int i = 0;

    while (i < no->length && no->comp(value, no->index[i]) != 0) {
        i++;
    }

    Bplus_find(no->p[i], value);
}

node **Bplus_findInterval(node *no, void* low, void* high) {
    if (!no) {
        printf("Árvore vazia.\n");
        return NULL;
    }

    if (!no->leaf) {
        int i = 0;
        while (i < no->length && no->comp(low, no->index[i]) == 2) {
            i++;
        }
        return Bplus_findInterval(no->p[i], low, high);
    }

    node **retorno;
    node *no_inicio = no, *no_fim = no;
    int cont = 0, len_retorno = 0;

    while (no_fim != NULL) {
        if (no->comp(no_fim->index[0], high) == 2) {
            break;
        }
        cont++;

        no_fim = no_fim->rigth;
    }

    retorno = malloc((cont + 1) * sizeof(node*));

    for (node* i = no_inicio; i != no_fim; i = i->rigth)
    {
        retorno[len_retorno++] = i;
    }
    retorno[len_retorno] = NULL;
    
    return retorno;
}