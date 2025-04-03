//ARVORE B+
//GRUPO: MIGUEL PERES; GABRIEL MELO; PAULO ANDRE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Numero maximo de chaves -> order -1 | Numero maximo dos ponteiros por nó 
#define ORDER 3

//Valores das folhas
typedef struct record {
    int value;
} record;

//struct para Nós
typedef struct node {
    void **pointers;      //ponteiros para as folhas ou nós filhos
    int *keys;            //chaves dos nós
    struct node *parent;  //ponteiro para o nó pai | para o split
    bool is_leaf;         //se o nó é folha ou interno, àqueles que possuem apenas os índices/chaves
    int num_keys;         //conta o número de chaves por no
    struct node *next;    //encadeia as folhas em lista
} node;

int order = ORDER;           //quantidade maxima de ponteiros por nó e de chaves 
node *queue = NULL;          //a fila encadeada da arvore
bool verbose_output = false; //mostra apenas o conteudo necessário da arvore

//Enqueue
void enqueue(node *new_node);

//Dequeue
node *dequeue(void);
int height(node *const root);
int pathToLeaves(node *const root, node *child);
void printLeaves(node *const root);
void printTree(node *const root);
void findAndPrint(node *const root, int key, bool verbose);
void findAndPrintRange(node *const root, int range1, int range2, bool verbose);
int findRange(node *const root, int key_start, int key_end, bool verbose, int returned_keys[], void *returned_pointers[]);
node *findLeaf(node *const root, int key, bool verbose);
record *find(node *root, int key, bool verbose, node **leaf_out);
int cut(int length);

record *makeRecord(int value);
node *makeNode(void);
node *makeLeaf(void);
int getLeftIndex(node *parent, node *left);
node *insertIntoLeaf(node *leaf, int key, record *pointer);
node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key, record *pointer);
node *insertIntoNode(node *root, node *parent, int left_index, int key, node *right);
node *insertIntoNodeAfterSplitting(node *root, node *parent, int left_index, int key, node *right);
node *insertIntoParent(node *root, node *left, int key, node *right);
node *insertIntoNewRoot(node *left, int key, node *right);
node *startNewTree(int key, record *pointer);
node *insert(node *root, int key, int value);

//adições de nos no final da fila
void enqueue(node *new_node) {
    node *c;                     //ponteiro auxiliar
    //se não haver nenhum elemento na fila
    if (queue == NULL) {
        queue = new_node;
        queue->next = NULL;      //vai apontar para NULL
    } else {
        c = queue;
        //percorre a fila até chegar no ultimo elemento
        while (c->next != NULL) {
            c = c->next;
        }
        //insere no ultimo elemento
        c->next = new_node;
        new_node->next = NULL;
    }
}

//remoção de nós do inicio da fila
node *dequeue(void) {
    node *n = queue;
    queue = queue->next;
    n->next = NULL;
    return n;
}

//Imprime as folhas da arvore
void printLeaves(node *const root) {
    if (root == NULL) {
        printf("Empty tree.\n");
        return;
    }
    int i;
    node *c = root;
    //até chegar no primeiro elemento da fila
    while (!c->is_leaf)
        c = c->pointers[0];
    while (true) {
        //imprimir nós folhas
        for (i = 0; i < c->num_keys; i++) {
            if (verbose_output)
                printf("%p ", c->pointers[i]);
            printf("%d ", c->keys[i]);
        }
        if (verbose_output)
            printf("%p ", c->pointers[order - 1]);
        
        //ir para a próxima folha, se tiver
        if (c->pointers[order - 1] != NULL) {
            printf(" | ");
            c = c->pointers[order - 1];
        }else
            break;
    }
    printf("\n");
}

//altura | número de níveis da arvore
int height(node *const root) {
    int h = 0;              //contador 
    node *c = root;         //raiz
    while (!c->is_leaf) {
        c = c->pointers[0];
        h++;                //acrecenta a h até chegar a um nó folha
    }
    return h;
}

//quantidades de saltos que um nó filho dá até chegar na raiz
int pathToLeaves(node *const root, node *child) {
    int length = 0;     //contador
    node *c = child;    //filho
    while (c != root) {
        c = c->parent;  
        length++;       //acrescenta até chegar na raiz
    }
    return length;
}

//imprime a arvore completa
void printTree(node *const root) {
    node *n = NULL;
    int i = 0;
    int rank = 0;
    int new_rank = 0;

    if (root == NULL) {
        printf("Empty tree.\n");
        return;
    }
    queue = NULL;
    enqueue(root);
    while (queue != NULL) {
        n = dequeue();
        if (n->parent != NULL && n == n->parent->pointers[0]) {
        new_rank = pathToLeaves(root, n);
        if (new_rank != rank) {
            rank = new_rank;
            printf("\n");
        }
        }
        if (verbose_output)
        printf("(%p)", n);
        for (i = 0; i < n->num_keys; i++) {
        if (verbose_output)
            printf("%p ", n->pointers[i]);
        printf("%d ", n->keys[i]);
        }
        if (!n->is_leaf)
        for (i = 0; i <= n->num_keys; i++)
            enqueue(n->pointers[i]);
        if (verbose_output) {
        if (n->is_leaf)
            printf("%p ", n->pointers[order - 1]);
        else
            printf("%p ", n->pointers[n->num_keys]);
        }
        printf("| ");
    }
    printf("\n");
}

//Imprime o valor buscado
void findAndPrint(node *const root, int key, bool verbose) {
    node *leaf = NULL;
    record *r = find(root, key, verbose, NULL);
    if (r == NULL)
        printf("Record not found under key %d.\n", key);
    else
        printf("Record at %p -- key %d, value %d.\n", r, key, r->value);
}

//Imprime valores de findRange
void findAndPrintRange(node *const root, int key_start, int key_end, bool verbose) {
    int i;
    int array_size = key_end - key_start + 1;
    int returned_keys[array_size];
    void *returned_pointers[array_size];
    int num_found = findRange(root, key_start, key_end, verbose,
                    returned_keys, returned_pointers);
    if (!num_found)
        printf("None found.\n");
    else {
        for (i = 0; i < num_found; i++)
        printf("Key: %d   Location: %p  Value: %d\n",
            returned_keys[i],
            returned_pointers[i],
            ((record *)
                returned_pointers[i])
                ->value);
    }
}

//busca valor
int findRange(node *const root, int key_start, int key_end, bool verbose, int returned_keys[], void *returned_pointers[]) {
    int i, num_found;
    num_found = 0;
    node *n = findLeaf(root, key_start, verbose);
    if (n == NULL)
        return 0;
    for (i = 0; i < n->num_keys && n->keys[i] < key_start; i++)
        ;
    if (i == n->num_keys)
        return 0;
    while (n != NULL) {
        for (; i < n->num_keys && n->keys[i] <= key_end; i++) {
        returned_keys[num_found] = n->keys[i];
        returned_pointers[num_found] = n->pointers[i];
        num_found++;
        }
        n = n->pointers[order - 1];
        i = 0;
    }
    return num_found;
}

// Busca valor em uma folha
node *findLeaf(node *const root, int key, bool verbose) {
    if (root == NULL) {
        if (verbose)
        printf("Empty tree.\n");
        return root;
    }
    int i = 0;
    node *c = root;
    //até chegar a uma folha
    while (!c->is_leaf) {
        if (verbose) {
        printf("[");
        for (i = 0; i < c->num_keys - 1; i++)
            printf("%d ", c->keys[i]);
        printf("%d] ", c->keys[i]);
        }
        i = 0;
        while (i < c->num_keys) {
        if (key >= c->keys[i])
            i++;
        else
            break;
        }
        if (verbose)
        printf("%d ->\n", i);
        c = (node *)c->pointers[i];
    }
    if (verbose) {
        printf("Leaf [");
        for (i = 0; i < c->num_keys - 1; i++)
        printf("%d ", c->keys[i]);
        printf("%d] ->\n", c->keys[i]);
    }
    return c;
}

//busca de valor para findAndPrint
record *find(node *root, int key, bool verbose, node **leaf_out) {
    if (root == NULL) {
        if (leaf_out != NULL) {
        *leaf_out = NULL;
        }
        return NULL;
    }

    int i = 0;
    node *leaf = NULL;

    leaf = findLeaf(root, key, verbose);

    for (i = 0; i < leaf->num_keys; i++)
        if (leaf->keys[i] == key)
        break;
    if (leaf_out != NULL) {
        *leaf_out = leaf;
    }
    if (i == leaf->num_keys)
        return NULL;
    else
        return (record *)leaf->pointers[i];
}

//determina a divisão das chaves para o split
int cut(int length) {
    if (length % 2 == 0)
        return length / 2;
    else
        return length / 2 + 1;
}

//cria/aloca um novo espaço para dados
record *makeRecord(int value) {
    record *new_record = (record *)malloc(sizeof(record));
    if (new_record == NULL) {
        perror("Record creation.");
        exit(EXIT_FAILURE);
    } else {
        new_record->value = value;
    }
    return new_record;
}

//cria um novo nó interno
node *makeNode(void) {
    node *new_node;
    new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        perror("Node creation.");
        exit(EXIT_FAILURE);
    }
    new_node->keys = malloc((order - 1) * sizeof(int));
    if (new_node->keys == NULL) {
        perror("New node keys array.");
        exit(EXIT_FAILURE);
    }
    new_node->pointers = malloc(order * sizeof(void *));
    if (new_node->pointers == NULL) {
        perror("New node pointers array.");
        exit(EXIT_FAILURE);
    }
    new_node->is_leaf = false;
    new_node->num_keys = 0;
    new_node->parent = NULL;
    new_node->next = NULL;
    return new_node;
}

//cria um nó folha 
node *makeLeaf(void) {
    node *leaf = makeNode();
    leaf->is_leaf = true;
    return leaf;
}

//auxilia a posição em que o novo nó vai ficar
int getLeftIndex(node *parent, node *left) {
    int left_index = 0;
    while (left_index <= parent->num_keys &&
        parent->pointers[left_index] != left)
        left_index++;
    return left_index;
}

//inserir quando a folha estiver vazia
node *insertIntoLeaf(node *leaf, int key, record *pointer) {
    int i, insertion_point;

    insertion_point = 0;
    while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
        insertion_point++;

    for (i = leaf->num_keys; i > insertion_point; i--) {
        leaf->keys[i] = leaf->keys[i - 1];
        leaf->pointers[i] = leaf->pointers[i - 1];
    }
    leaf->keys[insertion_point] = key;
    leaf->pointers[insertion_point] = pointer;
    leaf->num_keys++;
    return leaf;
}

//inserir quando estiver cheia | faz o split
node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key, record *pointer) {
    node *new_leaf;
    int *temp_keys;
    void **temp_pointers;
    int insertion_index, split, new_key, i, j;

    new_leaf = makeLeaf();

    temp_keys = malloc(order * sizeof(int));
    if (temp_keys == NULL) {
        perror("Temporary keys array.");
        exit(EXIT_FAILURE);
    }

    temp_pointers = malloc(order * sizeof(void *));
    if (temp_pointers == NULL) {
        perror("Temporary pointers array.");
        exit(EXIT_FAILURE);
    }

    insertion_index = 0;
    while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
        insertion_index++;

    for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
        if (j == insertion_index)
        j++;
        temp_keys[j] = leaf->keys[i];
        temp_pointers[j] = leaf->pointers[i];
    }

    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = pointer;

    leaf->num_keys = 0;

    split = cut(order - 1);

    for (i = 0; i < split; i++) {
        leaf->pointers[i] = temp_pointers[i];
        leaf->keys[i] = temp_keys[i];
        leaf->num_keys++;
    }

    for (i = split, j = 0; i < order; i++, j++) {
        new_leaf->pointers[j] = temp_pointers[i];
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->num_keys++;
    }

    free(temp_pointers);
    free(temp_keys);

    new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
    leaf->pointers[order - 1] = new_leaf;

    for (i = leaf->num_keys; i < order - 1; i++)
        leaf->pointers[i] = NULL;
    for (i = new_leaf->num_keys; i < order - 1; i++)
        new_leaf->pointers[i] = NULL;

    new_leaf->parent = leaf->parent;
    new_key = new_leaf->keys[0];

    return insertIntoParent(root, leaf, new_key, new_leaf);
}

//inserir em nó interno
node *insertIntoNode(node *root, node *n, int left_index, int key, node *right) {
    int i;

    for (i = n->num_keys; i > left_index; i--) {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }
    n->pointers[left_index + 1] = right;
    n->keys[left_index] = key;
    n->num_keys++;
    return root;
}

//split do nó interno
node *insertIntoNodeAfterSplitting(node *root, node *old_node, int left_index, int key, node *right) {
    int i, j, split, k_prime;
    node *new_node, *child;
    int *temp_keys;
    node **temp_pointers;

    temp_pointers = malloc((order + 1) * sizeof(node *));
    if (temp_pointers == NULL) {
        exit(EXIT_FAILURE);
    }
    temp_keys = malloc(order * sizeof(int));
    if (temp_keys == NULL) {
        exit(EXIT_FAILURE);
    }

    for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
        if (j == left_index + 1)
        j++;
        temp_pointers[j] = old_node->pointers[i];
    }

    for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
        if (j == left_index)
        j++;
        temp_keys[j] = old_node->keys[i];
    }

    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

    split = cut(order);
    new_node = makeNode();
    old_node->num_keys = 0;
    for (i = 0; i < split - 1; i++) {
        old_node->pointers[i] = temp_pointers[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }
    old_node->pointers[i] = temp_pointers[i];
    k_prime = temp_keys[split - 1];
    for (++i, j = 0; i < order; i++, j++) {
        new_node->pointers[j] = temp_pointers[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }
    new_node->pointers[j] = temp_pointers[i];
    free(temp_pointers);
    free(temp_keys);
    new_node->parent = old_node->parent;
    for (i = 0; i <= new_node->num_keys; i++) {
        child = new_node->pointers[i];
        child->parent = new_node;
    }

    return insertIntoParent(root, old_node, k_prime, new_node);
}

//inserção no nó pai
node *insertIntoParent(node *root, node *left, int key, node *right) {
    int left_index;
    node *parent;

    parent = left->parent;

    if (parent == NULL)
        return insertIntoNewRoot(left, key, right);

    left_index = getLeftIndex(parent, left);

    if (parent->num_keys < order - 1)
        return insertIntoNode(root, parent, left_index, key, right);

    return insertIntoNodeAfterSplitting(root, parent, left_index, key, right);
}

//insere nova raiz após splits que mudam a altura da arvore
node *insertIntoNewRoot(node *left, int key, node *right) {
    node *root = makeNode();
    root->keys[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    root->parent = NULL;
    left->parent = root;
    right->parent = root;
    return root;
}

//cria o primeiro nó
node *startNewTree(int key, record *pointer) {
    node *root = makeLeaf();
    root->keys[0] = key;
    root->pointers[0] = pointer;
    root->pointers[order - 1] = NULL;
    root->parent = NULL;
    root->num_keys++;
    return root;
}

//inserção geral
node *insert(node *root, int key, int value) {
    record *record_pointer = NULL;
    node *leaf = NULL;

    //verificando se o valor é repetido
    record_pointer = find(root, key, false, NULL);
    if (record_pointer != NULL) {
        record_pointer->value = value;             //atualiza o valor da chave se o valor for repetido
        return root;
    }

    //criando/alocando um novo espaço
    record_pointer = makeRecord(value);

    //arvore vazia
    if (root == NULL)
        return startNewTree(key, record_pointer);

    leaf = findLeaf(root, key, false);

    //inserção padrão ou com split
    if (leaf->num_keys < order - 1) {
        leaf = insertIntoLeaf(leaf, key, record_pointer);
        return root;
    }

    return insertIntoLeafAfterSplitting(root, leaf, key, record_pointer);
}

int main() {
    node *root;
    char instruction;

    root = NULL;

    //inserção de dados
    root = insert(root, 5, 33);
    root = insert(root, 15, 21);
    root = insert(root, 25, 31);
    root = insert(root, 35, 41);
    root = insert(root, 45, 10);

    printTree(root);

    //busca a chave do elemento 15
    findAndPrint(root, 15, instruction = 'a');

    return 0;
}
