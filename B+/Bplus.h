typedef enum { INT , FLOAT , STRING } Type;

#define ORDER 8

/* typedef struct Dados {
    char **dados;
    int length;
} Dados; */

typedef struct Data {
    Type type;
    char value[16];
} Data;

typedef struct Value {
    node_bplus *rigth;
    node_bplus *left;
    Data data
} Value;

typedef struct node_bplus
{
    bool folha;
    Value *values[ORDER];
} node_bplus;

/* typedef struct {
    int f_local;
    int length;
    int f_rigth;
    int f_left;
} leaf; */


typedef struct Bplus {
//    int id;
    node_bplus *root;
    Type type;
//    char local[16];
} Bplus;

Bplus *Bplus_alloc(Type);
void Bplus_free(Bplus *tree);
void Bplus_insert(Bplus *tree, char *value);
void Bplus_find(/*  */);
void Bplus_findInterval(/*  */);
void Bplus_remove(Bplus *tree, void *value);

//void *Bplus_search(Bplus *tree, void *value);
