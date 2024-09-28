#define ORDER 7

typedef struct node {
    void* index[ORDER]; // 0| a |1| b |2| c |3| d |4| e |5| f |6| g |7|
    int length;

    struct node *befor;
    struct node *p[ORDER+1];

    bool leaf;
    struct node *left;
    struct node *rigth;

    int (*comp)(void*a, void*b); // 1 se a for igual que b, 0 se for menor, 2 se for mair
} node;

typedef struct Bplus {
    node *root;
    int (*comp)(void*a, void*b);
} Bplus;

Bplus *Bplus_alloc(int (*comp)(void*a, void*b));
void Bplus_free(Bplus *tree);
void Bplus_insert(Bplus *tree, void* value);
node *Bplus_find(node *no, void* value);
node **Bplus_findInterval(node *no, void* low, void* high);
void Bplus_remove(Bplus *tree, void* value);
void Bplus_print(Bplus *tree, void (*print)(void*));
void print_node(node* no, void(*print)(void*));