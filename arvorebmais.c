//ARVORE B+
//GRUPO: MIGUEL PERES; GABRIEL MELO; PAULO ANDRE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // para variáveis boleanas | bool

// para auxiliar nos splits:
#define MAX_CHAVES 3               
#define MIN_CHAVES MAX_CHAVES/2    

// struct para nós:
typedef struct no {
    bool e_folha;
    int num_chaves;
    int chaves[MAX_CHAVES + 1];
    union {
        struct no *filho[MAX_CHAVES + 2];
        struct {
            void *valor[MAX_CHAVES + 1];
            struct no *prox;
        };
    };
    struct no *pai;
} No;

// inicializando raiz com NULL
No *raiz = NULL;

// struct para auxiliar na impressão BFS:
typedef struct QueueNo {
    No *no_arvore;
    struct QueueNo *prox;
} QueueNo;

// struct para auxiliar no Enqueue e Dequeue:
typedef struct {
    QueueNo *inicio;
    QueueNo *fim;
} Queue;

No *CriarNo(bool e_folha);
void Inserir(int chave);
void Inserir_na_Folha(No *folha, int chave);
void Inserir_no_Pai(No *esq, int chave, No *dir);
void Enqueue(Queue *q, No *no_arvore);
No *Dequeue(Queue *q);
bool QueueVazio(Queue *q);
void SplitFolha(No *folha);
void SplitInterno(No *no);
No *LocalizaFolha(No *no, int chave);
void BuscaChave(int chave);
void ImprimirBFS(No *raiz);

// função para criação de nós:
No *CriarNo(bool e_folha) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->e_folha = e_folha;
    novoNo->num_chaves = 0;
    novoNo->pai = NULL;
    
    if (e_folha) {
        novoNo->prox = NULL;
    } else {
        for (int i = 0; i <= MAX_CHAVES + 1; i++) {
            novoNo->filho[i] = NULL;
        }
    }
    
    return novoNo;
}

// função para localizar um nó folha | auxilia na busca e inserção:
No *LocalizaFolha(No *no, int chave) {
    if (no == NULL) return NULL;
    
    while (!no->e_folha) {
        int i = 0;
        while (i < no->num_chaves && chave >= no->chaves[i]) {
            i++;
        }
        no = no->filho[i];
    }
    
    return no;
}

// procedimento para inserir uma chave em um nó folha:
void Inserir_na_Folha(No *folha, int chave) {
    int i = folha->num_chaves - 1;
    
    while (i >= 0 && folha->chaves[i] > chave) {
        folha->chaves[i + 1] = folha->chaves[i];
        i--;
    }
    
    folha->chaves[i + 1] = chave;
    folha->num_chaves++;
}

// procedimento para dividir nó folha saturado:
void SplitFolha(No *folha) {
    No *novaFolha = CriarNo(true);
    int split_pos = (folha->num_chaves + 1) / 2;
    
    // copia metade das chaves para a nova folha
    for (int i = split_pos; i < folha->num_chaves; i++) {
        novaFolha->chaves[i - split_pos] = folha->chaves[i];
    }
    
    novaFolha->num_chaves = folha->num_chaves - split_pos;
    folha->num_chaves = split_pos;
    
    // atualiza os ponteiros das folhas
    novaFolha->prox = folha->prox;
    folha->prox = novaFolha;
    novaFolha->pai = folha->pai;
    
    // insere no pai a primeira chave da nova folha
    Inserir_no_Pai(folha, novaFolha->chaves[0], novaFolha);
}

// procedimento para inserir uma chave e um ponteiro para o nó direito no pai do nó esquerdo
void Inserir_no_Pai(No *esq, int chave, No *dir) {
    No *pai = esq->pai;
    
    if (pai == NULL) {
        // criar nova raiz
        No *novaRaiz = CriarNo(false);
        novaRaiz->chaves[0] = chave;
        novaRaiz->filho[0] = esq;
        novaRaiz->filho[1] = dir;
        novaRaiz->num_chaves = 1;
        esq->pai = novaRaiz;
        dir->pai = novaRaiz;
        raiz = novaRaiz;
        return;
    }
    
    // encontrar posição para inserir no pai
    int pos = 0;
    while (pos < pai->num_chaves && chave > pai->chaves[pos]) {
        pos++;
    }
    
    // abrir espaço para a nova chave
    for (int i = pai->num_chaves; i > pos; i--) {
        pai->chaves[i] = pai->chaves[i - 1];
    }
    for (int i = pai->num_chaves + 1; i > pos + 1; i--) {
        pai->filho[i] = pai->filho[i - 1];
    }
    
    // inserir a chave e o ponteiro
    pai->chaves[pos] = chave;
    pai->filho[pos + 1] = dir;
    pai->num_chaves++;
    
    if (pai->num_chaves > MAX_CHAVES) {
        SplitInterno(pai);
    }
}

// procedimento para dividir nó interno saturado:
void SplitInterno(No *no) {
    No *novoNo = CriarNo(false);
    int split_pos = no->num_chaves / 2;
    int chavePromovida = no->chaves[split_pos];
    
    // copia metade das chaves e ponteiros para o novo nó
    for (int i = split_pos + 1; i < no->num_chaves; i++) {
        novoNo->chaves[i - split_pos - 1] = no->chaves[i];
    }
    for (int i = split_pos + 1; i <= no->num_chaves; i++) {
        novoNo->filho[i - split_pos - 1] = no->filho[i];
        no->filho[i]->pai = novoNo;
    }
    
    novoNo->num_chaves = no->num_chaves - split_pos - 1;
    no->num_chaves = split_pos;
    novoNo->pai = no->pai;
    
    // insere no pai a chave promovida
    Inserir_no_Pai(no, chavePromovida, novoNo);
}

// procedimento para inserir um elemento:
void Inserir(int chave) {
    if (raiz == NULL) {
        raiz = CriarNo(true);
        Inserir_na_Folha(raiz, chave);
        return;
    }
    
    No *folha = LocalizaFolha(raiz, chave);
    
    // verificar se a chave já existe
    for (int i = 0; i < folha->num_chaves; i++) {
        if (folha->chaves[i] == chave) {
            printf("Chave %d já existe na árvore.\n", chave);
            return;
        }
    }
    
    if (folha->num_chaves < MAX_CHAVES) {
        Inserir_na_Folha(folha, chave);
    } else {
        Inserir_na_Folha(folha, chave); // inserir primeiro (fica temporariamente com MAX+1)
        SplitFolha(folha);              // depois dividir
    }
}

// procedimento para buscar chaves:
void BuscaChave(int chave) {
    if (raiz == NULL) {
        printf("Árvore vazia.\n");
        return;
    }
    
    No *folha = LocalizaFolha(raiz, chave);
    
    for (int i = 0; i < folha->num_chaves; i++) {
        if (folha->chaves[i] == chave) {
            printf("Chave %d encontrada.\n", chave);
            return;
        }
    }
    
    printf("Chave %d não encontrada.\n", chave);
}

// procedimento para inserir no fim da fila das folhas:
void Enqueue(Queue *q, No *no_arvore) {
    QueueNo *novoNo = (QueueNo *)malloc(sizeof(QueueNo));
    novoNo->no_arvore = no_arvore;
    novoNo->prox = NULL;
    
    if (q->fim == NULL) {
        q->inicio = q->fim = novoNo;
    } else {
        q->fim->prox = novoNo;
        q->fim = novoNo;
    }
}

// procedimento para remover no inicio da fila das folhas:
No *Dequeue(Queue *q) {
    if (q->inicio == NULL) return NULL;
    
    QueueNo *temp = q->inicio;
    No *no = temp->no_arvore;
    q->inicio = q->inicio->prox;
    
    if (q->inicio == NULL) {
        q->fim = NULL;
    }
    
    free(temp);
    return no;
}

// função que verifica se fila das folhas está vazia:
bool QueueVazio(Queue *q) {
    return q->inicio == NULL;
}

// procedimento para imprimir arvore em níveis:
void ImprimirBFS(No *raiz) {
    if (raiz == NULL) {
        printf("Árvore vazia.\n");
        return;
    }

    Queue q = {NULL, NULL};
    Enqueue(&q, raiz);
    
    while (!QueueVazio(&q)) {
        int tam_nivel = 0;
        Queue temp = {NULL, NULL};
        
        // processar todos os nós do nível atual
        while (!QueueVazio(&q)) {
            No *noAtual = Dequeue(&q);
            tam_nivel++;
            
            // imprimir o nó atual
            printf("[");
            for (int i = 0; i < noAtual->num_chaves; i++) {
                printf("%d", noAtual->chaves[i]);
                if (i < noAtual->num_chaves - 1) printf("|");
            }
            printf("] ");
            
            // enfileirar os filhos para o próximo nível
            if (!noAtual->e_folha) {
                for (int i = 0; i <= noAtual->num_chaves; i++) {
                    if (noAtual->filho[i] != NULL) {
                        Enqueue(&temp, noAtual->filho[i]);
                    }
                }
            }
        }
        
        printf("\n"); // nova linha para o próximo nível
        
        // transferir os nós do próximo nível para a fila principal
        while (!QueueVazio(&temp)) {
            Enqueue(&q, Dequeue(&temp));
        }
    }
}

int main() {
    // inserindo valores à arvore
    Inserir(79);
    Inserir(24);
    Inserir(32);
    Inserir(134);
    Inserir(0);
    Inserir(17);
    Inserir(30);
    Inserir(33);
    Inserir(132);
    Inserir(150);
    
    // exibindo:
    printf("Estrutura da Árvore B+ (BFS por níveis):\n");
    ImprimirBFS(raiz);
    
    // buscando chaves:
    printf("\nBusca:\n");
    BuscaChave(134);
    BuscaChave(12);
    
    return 0;
}