#define ORDER 7

typedef struct node {
    int index[ORDER]; // 0| a |1| b |2| c |3
    int length;

    struct node *befor;
    struct node *p[4];

    bool leaf;
    struct node *left;
    struct node *rigth;
} node;

typedef struct Bplus {
    node *root;
} Bplus;

Bplus *Bplus_alloc();
void Bplus_free(Bplus *tree);
void Bplus_insert(Bplus *tree, int value);
void Bplus_find(node *no, int value);
void Bplus_findInterval(node *no, int low, int high);
void Bplus_remove(Bplus *tree, int value);

void print_node(node* no);
//void *Bplus_search(Bplus *tree, void *value);
