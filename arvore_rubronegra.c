//ARVORE RUBRO-NEGRA
//GRUPO: MIGUEL PERES; GABRIEL MELO; PAULO ANDRE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição da cor dos nós da árvore rubro-negra
typedef enum { 
    VERMELHO, 
    PRETO 
} Cor;

// Estrutura do nó da árvore rubro-negra
typedef struct No {
    int chave;
    Cor cor;
    struct No *esq, *dir;
} No;

// Função auxiliar para criar nó
No* criar_no(int chave) {
    No *novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    novo->chave = chave;
    novo->cor = VERMELHO; // Novo nó é sempre vermelho
    novo->esq = novo->dir = NULL;
    return novo;
}

// Verifica se um nó é vermelho
bool eh_vermelho(No *no) {
    if (no == NULL) return false;
    return no->cor == VERMELHO;
}

// Funções de rotação
No* rotacionar_esquerda(No *A) {
    No *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

No* rotacionar_direita(No *A) {
    No *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

// Inverte as cores de um nó e seus filhos
void inverter_cores(No *no) {
    no->cor = (no->cor == VERMELHO) ? PRETO : VERMELHO;
    if (no->esq) no->esq->cor = (no->esq->cor == VERMELHO) ? PRETO : VERMELHO;
    if (no->dir) no->dir->cor = (no->dir->cor == VERMELHO) ? PRETO : VERMELHO;
}

// Função de balanceamento
No* balancear(No *no) {
    // Caso 1: Filho direito é vermelho e esquerdo é preto
    if (eh_vermelho(no->dir) && !eh_vermelho(no->esq))
        no = rotacionar_esquerda(no);
    
    // Caso 2: Filho esquerdo e neto esquerdo são vermelhos
    if (eh_vermelho(no->esq) && eh_vermelho(no->esq->esq))
        no = rotacionar_direita(no);
    
    // Caso 3: Ambos os filhos são vermelhos (apenas inverter cores se o nó atual for preto)
    if (eh_vermelho(no->esq) && eh_vermelho(no->dir)) {
        if (!eh_vermelho(no)) {  // Só inverte se o nó atual for preto
            inverter_cores(no);
        }
    }
    
    return no;
}

// Função de inserção
No* inserir_no(No *no, int chave) {
    if (no == NULL) return criar_no(chave);
    
    if (chave < no->chave)
        no->esq = inserir_no(no->esq, chave);
    else if (chave > no->chave)
        no->dir = inserir_no(no->dir, chave);
    else
        return no; // Chave já existe
    
    // Balanceamento
    // Caso 1: Filho direito vermelho e esquerdo preto
    if (eh_vermelho(no->dir) && !eh_vermelho(no->esq))
        no = rotacionar_esquerda(no);
    
    // Caso 2: Filho esquerdo e neto esquerdo vermelhos
    if (eh_vermelho(no->esq) && eh_vermelho(no->esq->esq))
        no = rotacionar_direita(no);
    
    // Caso 3: Ambos os filhos vermelhos
    if (eh_vermelho(no->esq) && eh_vermelho(no->dir)) {
        // Só inverte cores se o nó atual for preto
        if (!eh_vermelho(no)) {
            inverter_cores(no);
        }
    }
    
    return no;
}

// Função que garante a raiz PRETA
No* inserir(No *raiz, int chave) {
    raiz = inserir_no(raiz, chave);
    raiz->cor = PRETO; // Raiz sempre preta
    return raiz;
}

// Funções para remoção
// Funções auxiliares para mover vermelho
No* mover_vermelho_esquerda(No *no) {
    inverter_cores(no);
    
    if (eh_vermelho(no->dir->esq)) {
        no->dir = rotacionar_direita(no->dir);
        no = rotacionar_esquerda(no);
        inverter_cores(no);
    }
    return no;
}

// Função auxiliar para mover vermelho à direita
No* mover_vermelho_direita(No *no) {
    inverter_cores(no);
    
    if (eh_vermelho(no->esq->esq)) {
        no = rotacionar_direita(no);
        inverter_cores(no);
    }
    return no;
}

// Função auxiliar para encontrar o nó mínimo
No* encontrar_minimo(No *no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

// Função auxiliar para remover o nó mínimo
No* remover_minimo(No *no) {
    if (no->esq == NULL) {
        free(no);
        return NULL;
    }
    
    if (!eh_vermelho(no->esq) && !eh_vermelho(no->esq->esq))
        no = mover_vermelho_esquerda(no);
    
    no->esq = remover_minimo(no->esq);
    return balancear(no);
}

// Função auxiliar para remover um nó
No* remover_no(No *no, int chave) {
    if (chave < no->chave) {
        if (!eh_vermelho(no->esq) && !eh_vermelho(no->esq->esq))
            no = mover_vermelho_esquerda(no);
        no->esq = remover_no(no->esq, chave);
    } else {
        if (eh_vermelho(no->esq))
            no = rotacionar_direita(no);
            
        if (chave == no->chave && no->dir == NULL) {
            free(no);
            return NULL;
        }
        
        if (!eh_vermelho(no->dir) && !eh_vermelho(no->dir->esq))
            no = mover_vermelho_direita(no);
            
        if (chave == no->chave) {
            No *min = encontrar_minimo(no->dir);
            no->chave = min->chave;
            no->dir = remover_minimo(no->dir);
        } else {
            no->dir = remover_no(no->dir, chave);
        }
    }
    return balancear(no);
}

// Função de remoção principal
No* remover(No *raiz, int chave) {
    if (raiz == NULL) return NULL;
    
    raiz = remover_no(raiz, chave);
    if (raiz != NULL) raiz->cor = PRETO;
    return raiz;
}

// Função de busca
No* buscar(No *no, int chave) {
    if (no == NULL) return NULL;
    
    if (chave < no->chave)
        return buscar(no->esq, chave);
    else if (chave > no->chave)
        return buscar(no->dir, chave);
    else
        return no;
}

// Função para imprimir a árvore hierarquicamente
void imprimir_hierarquico_util(No *no, int nivel) {
    if (no == NULL) return;
    
    imprimir_hierarquico_util(no->dir, nivel + 1);
    
    for (int i = 0; i < nivel; i++)
        printf("    ");
    
    printf("%d(%c)\n", no->chave, no->cor == VERMELHO ? 'V' : 'P');
    
    imprimir_hierarquico_util(no->esq, nivel + 1);
}

// Função para imprimir a árvore hierarquicamente
void imprimir_hierarquico(No *raiz) {
    if (raiz == NULL) {
        printf("Árvore vazia!\n");
        return;
    }
    printf("\nESTRUTURA DA ÁRVORE:\n");
    imprimir_hierarquico_util(raiz, 0);
    printf("\n");
}

// Função para liberar memória
void liberar_arvore(No *no) {
    if (no == NULL) return;
    liberar_arvore(no->esq);
    liberar_arvore(no->dir);
    free(no);
}

int main() {
    No *arvore = NULL;
    int opcao, chave;
    
    do {
        printf("\n----------------------------------------\n");
        printf(" MENU ÁRVORE RUBRO-NEGRA\n");
        printf("----------------------------------------\n");
        printf("1. Inserir chave\n");
        printf("2. Remover chave\n");
        printf("3. Buscar chave\n");
        printf("4. Visualizar árvore\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                arvore = inserir(arvore, chave);
                printf("Chave %d inserida com sucesso!\n", chave);
                break;
                
            case 2:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                arvore = remover(arvore, chave);
                printf("Operação concluída.\n");
                break;
                
            case 3:
                printf("Digite a chave a ser buscada: ");
                scanf("%d", &chave);
                No *resultado = buscar(arvore, chave);
                if (resultado != NULL)
                    printf("Chave %d encontrada (Cor: %s)\n", chave, resultado->cor == VERMELHO ? "Vermelho" : "Preto");
                else
                    printf("Chave %d não encontrada.\n", chave);
                break;
                
            case 4:
                imprimir_hierarquico(arvore);
                break;
                
            case 0:
                printf("Encerrando programa...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    liberar_arvore(arvore);

    return 0;
}