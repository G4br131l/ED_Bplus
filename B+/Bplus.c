#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Bplus.h"

Bplus *Bplus_alloc(Type type) {
    Bplus *tree = malloc(sizeof(Bplus));
    if (!tree) {
        exit(1);
    }
    
    tree->type = type;
    tree->root = NULL;

    return tree;
}

void free_node(node_bplus *node) {
    free_node(node->left);
    free_node(node->rigth);
    free(node);
}

void Bplus_free(Bplus *tree) {
    if (!tree) {
        exit(1);
    }
    
    free_node(tree->root);
    free(tree);
}

void Bplus_insert(Bplus *tree, char *value, Type type) {
    if (!tree || !value || !type || type != tree->type)
    {
        exit(1);
    }
    

}