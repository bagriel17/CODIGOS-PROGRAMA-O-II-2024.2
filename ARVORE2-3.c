//código de implementação de arvore2-3. C.C 2024.2
//Seguintes funcionalidades: Inserção, Busca, Exclusão e Seleção de percurso

#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAVES 2       //Máximo de chaves por nó
#define MAX_FILHOS 3       //Máximo de filhos por nó

// Estrutura do nó da Árvore 2-3
typedef struct No {
    int chaves[MAX_CHAVES];   // Array de chaves
    struct No* filhos[MAX_FILHOS]; // Array de ponteiros para filhos
    int qtd;                  // Quantidade de chaves no nó
    int folha;                // Indica se o nó é folha (1 = folha, 0 = não folha)
} No;

// Função para criar um novo nó
No* criarNo(int folha) {
    No* no = (No*)malloc(sizeof(No)); // Aloca memória para o nó
    no->qtd = 0;                      // Inicializa a quantidade de chaves como 0
    no->folha = folha;                // Define se o nó é folha ou não
    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;         // Inicializa todos os filhos como NULL
    }
    return no;                        // Retorna o nó criado
}

// Função para buscar uma chave na árvore
int buscar(No* no, int chave) {
    int i = 0;
    // Encontra a posição onde a chave deveria estar
    while (i < no->qtd && chave > no->chaves[i]) {
        i++;
    }
    // Verifica se a chave foi encontrada
    if (i < no->qtd && chave == no->chaves[i]) {
        return 1; // Chave encontrada
    } else if (no->folha) {
        return 0; // Chave não encontrada (nó folha)
    } else {
        return buscar(no->filhos[i], chave); // Busca no filho apropriado
    }
}

// Função para exibir a árvore em ordem (In-Order)
void percursoEmOrdem(No* no) {
    if (no != NULL) {
        for (int i = 0; i < no->qtd; i++) {
            if (!no->folha) {
                percursoEmOrdem(no->filhos[i]); // Visita o filho esquerdo
            }
            printf("%d ", no->chaves[i]);      // Exibe a chave atual
        }
        if (!no->folha) {
            percursoEmOrdem(no->filhos[no->qtd]); // Visita o último filho
        }
    }
}

// Função para exibir a árvore por nível (Level-Order)
void percursoPorNivel(No* raiz) {
    if (raiz == NULL) return;

    No* fila[100]; // Fila para armazenar os nós
    int frente = 0, tras = 0;
    fila[tras++] = raiz; // Adiciona a raiz na fila

    // Processa a fila enquanto houver nós
    while (frente < tras) {
        No* no = fila[frente++]; // Remove o nó da fila
        for (int i = 0; i < no->qtd; i++) {
            printf("%d ", no->chaves[i]); // Exibe as chaves do nó
        }
        if (!no->folha) {
            // Adiciona os filhos na fila
            for (int i = 0; i <= no->qtd; i++) {
                if (no->filhos[i] != NULL) {
                    fila[tras++] = no->filhos[i];
                }
            }
        }
    }
}

// Função para inserir uma chave em um nó não cheio
void inserirEmNo(No* no, int chave, No* filhoDir) {
    int i = no->qtd - 1;
    // Encontra a posição correta para a nova chave
    while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i]; // Desloca as chaves para a direita
        no->filhos[i + 2] = no->filhos[i + 1]; // Desloca os filhos para a direita
        i--;
    }
    no->chaves[i + 1] = chave; // Insere a nova chave
    no->filhos[i + 2] = filhoDir; // Insere o filho direito
    no->qtd++; // Incrementa a quantidade de chaves
}

// Função para dividir um nó cheio
No* dividirNo(No* no, int* chavePromovida) {
    No* novoNo = criarNo(no->folha); // Cria um novo nó
    *chavePromovida = no->chaves[1]; // Promove a chave do meio
    novoNo->chaves[0] = no->chaves[2]; // Move a última chave para o novo nó
    novoNo->qtd = 1; // Atualiza a quantidade de chaves no novo nó
    no->qtd = 1; // Atualiza a quantidade de chaves no nó original

    if (!no->folha) {
        // Move os filhos para o novo nó
        novoNo->filhos[0] = no->filhos[2];
        novoNo->filhos[1] = no->filhos[3];
        no->filhos[2] = no->filhos[3] = NULL; // Limpa os ponteiros antigos
    }

    return novoNo; // Retorna o novo nó
}

// Função recursiva para inserir uma chave
No* inserirRecursivo(No* no, int chave, int* chavePromovida) {
    if (no->folha) {
        if (no->qtd < MAX_CHAVES) { // Verifica se o nó folha tem espaço
            inserirEmNo(no, chave, NULL); // Insere a chave
            return NULL;
        } else {
            // Nó folha está cheio, precisa dividir
            No* novoNo = criarNo(1);
            *chavePromovida = no->chaves[1]; // Promove a chave do meio
            novoNo->chaves[0] = no->chaves[2]; // Move a última chave para o novo nó
            novoNo->qtd = 1;
            no->qtd = 1;
            if (chave < *chavePromovida) {
                inserirEmNo(no, chave, NULL); // Insere no nó original
            } else {
                inserirEmNo(novoNo, chave, NULL); // Insere no novo nó
            }
            return novoNo; // Retorna o novo nó
        }
    } else {
        int i = no->qtd - 1;
        // Encontra o filho apropriado para a chave
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
        }
        i++;
        if (no->filhos[i] == NULL) {
            no->filhos[i] = criarNo(1); // Cria um novo nó folha se necessário
        }
        No* filhoNovo = inserirRecursivo(no->filhos[i], chave, chavePromovida);
        if (filhoNovo) {
            inserirEmNo(no, *chavePromovida, filhoNovo); // Insere a chave promovida
            if (no->qtd == MAX_CHAVES + 1) {
                return dividirNo(no, chavePromovida); // Divide o nó se necessário
            }
        }
        return NULL;
    }
}

// Função principal para inserir uma chave
No* inserir(No* raiz, int chave) {
    int chavePromovida;
    No* novoNo = inserirRecursivo(raiz, chave, &chavePromovida);
    if (!novoNo) return raiz; // Se não houve divisão, retorna a raiz original

    // Cria uma nova raiz se houve divisão
    No* novaRaiz = criarNo(0);
    novaRaiz->chaves[0] = chavePromovida;
    novaRaiz->filhos[0] = raiz;
    novaRaiz->filhos[1] = novoNo;
    novaRaiz->qtd = 1;
    return novaRaiz;
}

// Função para encontrar a chave mínima em uma subárvore
int encontrarMinimo(No* no) {
    while (!no->folha) {
        no = no->filhos[0]; // Desce até o filho mais à esquerda
    }
    return no->chaves[0]; // Retorna a chave mínima
}

// Função para remover uma chave (exclusão)
No* remover(No* no, int chave) {
    if (no == NULL) return NULL;

    int i = 0;
    // Encontra a posição da chave ou do filho apropriado
    while (i < no->qtd && chave > no->chaves[i]) {
        i++;
    }

    if (i < no->qtd && chave == no->chaves[i]) { // Chave encontrada no nó atual
        if (no->folha) {
            // Caso 1: A chave está em um nó folha
            for (int j = i; j < no->qtd - 1; j++) {
                no->chaves[j] = no->chaves[j + 1]; // Desloca as chaves para a esquerda
            }
            no->qtd--; // Atualiza a quantidade de chaves
        } else {
            // Caso 2: A chave está em um nó interno
            if (no->filhos[i]->qtd >= 1) {
                // Substituir pela chave predecessora
                int pred = encontrarMinimo(no->filhos[i]);
                no->chaves[i] = pred;
                no->filhos[i] = remover(no->filhos[i], pred);
            } else if (no->filhos[i + 1]->qtd >= 1) {
                // Substituir pela chave sucessora
                int succ = encontrarMinimo(no->filhos[i + 1]);
                no->chaves[i] = succ;
                no->filhos[i + 1] = remover(no->filhos[i + 1], succ);
            } else {
                // Caso 3: Fusão de nós
                No* filhoEsq = no->filhos[i];
                No* filhoDir = no->filhos[i + 1];
                filhoEsq->chaves[1] = no->chaves[i]; // Move a chave do pai para o filho esquerdo
                filhoEsq->chaves[2] = filhoDir->chaves[0]; // Move a chave do filho direito
                filhoEsq->qtd = 3; // Atualiza a quantidade de chaves
                for (int j = i; j < no->qtd - 1; j++) {
                    no->chaves[j] = no->chaves[j + 1]; // Desloca as chaves do pai
                    no->filhos[j + 1] = no->filhos[j + 2]; // Desloca os filhos do pai
                }
                no->qtd--; // Atualiza a quantidade de chaves do pai
                free(filhoDir); // Libera o filho direito
                no->filhos[i] = remover(filhoEsq, chave); // Remove a chave do filho esquerdo
            }
        }
    } else if (!no->folha) {
        // Caso 4: A chave não está no nó atual, buscar no filho apropriado
        no->filhos[i] = remover(no->filhos[i], chave);
    }

    // Redistribuição ou fusão após a remoção
    if (no->qtd == 0) {
        free(no); // Libera o nó se estiver vazio
        return NULL;
    }

    return no; // Retorna o nó atualizado
}

// Função principal
int main() {
    No* raiz = criarNo(1); // Cria a raiz como folha

    int opcao, valor;
    while (1) {
        printf("\n1. Inserir\n2. Buscar\n3. Exibir em Ordem\n4. Exibir por Nível\n5. Remover\n6. Sair\nEscolha: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Valor a buscar: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor)) {
                    printf("Chave %d encontrada na árvore.\n", valor);
                } else {
                    printf("Chave %d não encontrada na árvore.\n", valor);
                }
                break;
            case 3:
                printf("Percurso em Ordem: ");
                percursoEmOrdem(raiz);
                printf("\n");
                break;
            case 4:
                printf("Percurso por Nível: ");
                percursoPorNivel(raiz);
                printf("\n");
                break;
            case 5:
                printf("Valor a remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
            case 6:
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}
