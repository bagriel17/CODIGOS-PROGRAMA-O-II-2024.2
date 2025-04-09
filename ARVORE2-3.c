#include <stdio.h>
#include <stdlib.h>

/* CONSTANTES FUNDAMENTAIS */
#define MAX_CHAVES 2       // Máximo de chaves por nó (característica da árvore 2-3)
#define MAX_FILHOS 3       // Máximo de filhos por nó (um nó com 2 chaves tem 3 filhos)

/* ESTRUTURA DO NÓ */
typedef struct No {
    int chaves[MAX_CHAVES];   // Array para armazenar as chaves (valores)
    struct No* filhos[MAX_FILHOS]; // Array de ponteiros para os filhos
    int qtd;                  // Quantidade atual de chaves no nó
    int folha;                // Flag: 1 se é folha, 0 se é nó interno
    
    /* IMPORTANTE:
       - Nós folha: filhos são todos NULL
       - Nós internos: têm (qtd+1) filhos válidos
       - Sempre mantém: qtd <= MAX_CHAVES */
} No;

/* FUNÇÃO: criarNo
   Aloca e inicializa um novo nó da árvore */
No* criarNo(int folha) {
    No* no = (No*)malloc(sizeof(No));
    if (no == NULL) {
        fprintf(stderr, "Erro de alocação de memória!\n");
        exit(EXIT_FAILURE);
    }
    
    no->qtd = 0;
    no->folha = folha;
    
    // Inicializa todos os filhos como NULL
    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }
    
    return no;
}

/* FUNÇÃO: imprimirEspacos (auxiliar)
   Auxilia na exibição hierárquica da árvore */
void imprimirEspacos(int nivel) {
    for (int i = 0; i < nivel; i++) {
        printf("    ");  // 4 espaços por nível de profundidade
    }
}

/* FUNÇÃO: percursoEmOrdem
   Exibe a árvore em ordem (in-ordem) com indentação */
void percursoEmOrdem(No* no, int nivel) {
    if (no != NULL) {
        // Visita primeiro o filho mais à esquerda (se existir)
        if (!no->folha) {
            percursoEmOrdem(no->filhos[0], nivel + 1);
        }
        
        // Imprime o nó atual com indentação adequada
        imprimirEspacos(nivel);
        printf("(");
        for (int i = 0; i < no->qtd; i++) {
            printf("%d", no->chaves[i]);
            if (i < no->qtd - 1) printf("|");  // Separador entre chaves
        }
        printf(")\n");
        
        // Visita os filhos restantes (meio e direito)
        if (!no->folha) {
            for (int i = 1; i <= no->qtd; i++) {
                percursoEmOrdem(no->filhos[i], nivel + 1);
            }
        }
    }
}

/* FUNÇÃO: percursoPorNivel
   Exibe a árvore por níveis (BFS) */
void percursoPorNivel(No* raiz) {
    if (raiz == NULL) return;
    
    // Fila para armazenar os nós durante o percurso
    No* fila[100];
    int nivel[100];  // Armazena o nível de cada nó correspondente
    int frente = 0, tras = 0;
    
    // Inicia a fila com a raiz (nível 0)
    fila[tras] = raiz;
    nivel[tras++] = 0;
    
    int nivel_atual = -1;  // Controla o nível atual sendo impresso
    
    while (frente < tras) {
        No* no = fila[frente];
        int nl = nivel[frente++];
        
        // Se mudou de nível, imprime o cabeçalho
        if (nl != nivel_atual) {
            nivel_atual = nl;
            printf("\nNivel %d: ", nivel_atual);
        } else {
            printf("    ");  // Mesmo nível, apenas espaça
        }
        
        // Imprime as chaves do nó atual
        printf("[");
        for (int i = 0; i < no->qtd; i++) {
            printf("%d", no->chaves[i]);
            if (i < no->qtd - 1) printf("|");
        }
        printf("]");
        
        // Enfileira os filhos para processamento posterior
        if (!no->folha) {
            for (int i = 0; i <= no->qtd; i++) {
                if (no->filhos[i] != NULL) {
                    fila[tras] = no->filhos[i];
                    nivel[tras++] = nl + 1;
                }
            }
        }
    }
    printf("\n");
}

/* FUNÇÃO: buscar
   Busca uma chave na árvore (recursivamente) */
int buscar(No* no, int chave) {
    int i = 0;
    
    // Encontra a posição onde a chave deveria estar
    while (i < no->qtd && chave > no->chaves[i]) {
        i++;
    }
    
    // Verifica se encontrou a chave no nó atual
    if (i < no->qtd && chave == no->chaves[i]) {
        return 1;
    }
    // Se for folha e não encontrou, retorna não encontrado
    else if (no->folha) {
        return 0;
    }
    // Caso contrário, busca no filho apropriado
    else {
        return buscar(no->filhos[i], chave);
    }
}

/* FUNÇÃO: inserirEmNo (auxiliar)
   Insere uma chave em um nó que não está cheio */
void inserirEmNo(No* no, int chave, No* filhoDir) {
    int i = no->qtd - 1;
    
    // Encontra a posição correta e desloca elementos para a direita
    while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i];
        no->filhos[i + 2] = no->filhos[i + 1];
        i--;
    }
    
    // Insere a nova chave na posição correta
    no->chaves[i + 1] = chave;
    no->filhos[i + 2] = filhoDir;
    no->qtd++;
}

//* FUNÇÃO: dividirNo (corrigida) Divide um nó cheio e retorna o novo nó *//
No* dividirNo(No* no, int* chavePromovida) {
    No* novoNo = criarNo(no->folha);
    
    // A chave do meio é promovida
    *chavePromovida = no->chaves[1];
    
    // Novo nó recebe a maior chave
    novoNo->chaves[0] = no->chaves[2];
    novoNo->qtd = 1;
    
    // Nó original fica com a menor chave
    no->qtd = 1;
    
    // Se não for folha, redistribui os filhos corretamente
    if (!no->folha) {
        novoNo->filhos[0] = no->filhos[2];
        novoNo->filhos[1] = no->filhos[3];
        
        // Limpa os ponteiros no nó original
        no->filhos[2] = NULL;
        no->filhos[3] = NULL;
    }
    
    return novoNo;
}

/* FUNÇÃO: inserirRecursivo (corrigida)
   Função recursiva que realiza a inserção seguindo as regras da árvore 2-3 */
No* inserirRecursivo(No* no, int chave, int* chavePromovida) {
    if (no->folha) {
        // Caso 1: Inserção em nó folha
        if (no->qtd < MAX_CHAVES) {
            inserirEmNo(no, chave, NULL);
            return NULL;
        } else {
            // Nó folha está cheio - precisa dividir
            No* novoNo = criarNo(1);  // Cria nova folha
            
            // Determina qual chave promover (a do meio)
            if (chave < no->chaves[0]) {
                *chavePromovida = no->chaves[0];
                novoNo->chaves[0] = no->chaves[1];
                no->chaves[0] = chave;
            } else if (chave > no->chaves[1]) {
                *chavePromovida = no->chaves[1];
                novoNo->chaves[0] = chave;
            } else {
                *chavePromovida = chave;
                novoNo->chaves[0] = no->chaves[1];
            }
            
            no->qtd = 1;
            novoNo->qtd = 1;
            
            return novoNo;
        }
    } else {
        // Caso 2: Inserção em nó interno
        int i = 0;
        while (i < no->qtd && chave > no->chaves[i]) {
            i++;
        }
        
        // Insere recursivamente no filho apropriado
        No* filhoNovo = inserirRecursivo(no->filhos[i], chave, chavePromovida);
        
        if (filhoNovo != NULL) {
            // Se houve divisão no filho, insere a chave promovida no nó atual
            if (no->qtd < MAX_CHAVES) {
                inserirEmNo(no, *chavePromovida, filhoNovo);
                return NULL;
            } else {
                // Nó atual está cheio - precisa dividir
                No* novoNo = dividirNo(no, chavePromovida);
                
                // Decide onde colocar a nova chave promovida
                if (*chavePromovida < no->chaves[0]) {
                    // Insere no nó original
                    inserirEmNo(no, *chavePromovida, filhoNovo);
                } else {
                    // Insere no novo nó
                    inserirEmNo(novoNo, *chavePromovida, filhoNovo);
                }
                
                return novoNo;
            }
        }
        return NULL;
    }
}

/* FUNÇÃO: inserir (corrigida)
   Função principal de inserção na árvore 2-3 */
No* inserir(No* raiz, int chave) {
    int chavePromovida;
    No* novoNo = inserirRecursivo(raiz, chave, &chavePromovida);
    
    // Se houve divisão na raiz, cria uma nova raiz
    if (novoNo != NULL) {
        No* novaRaiz = criarNo(0);
        novaRaiz->chaves[0] = chavePromovida;
        novaRaiz->filhos[0] = raiz;
        novaRaiz->filhos[1] = novoNo;
        novaRaiz->qtd = 1;
        return novaRaiz;
    }
    
    return raiz;
}

/* FUNÇÃO: encontrarMinimo (auxiliar)
   Encontra a menor chave na subárvore */
int encontrarMinimo(No* no) {
    // Desce sempre pelo filho mais à esquerda
    while (!no->folha) {
        no = no->filhos[0];
    }
    
    // Retorna a primeira chave da folha
    return no->chaves[0];
}

/* FUNÇÃO: remover
   Remove uma chave da árvore (recursivamente) */
No* remover(No* no, int chave) {
    if (no == NULL) {
        return NULL;
    }
    
    int i = 0;
    
    // Encontra a posição da chave ou do filho apropriado
    while (i < no->qtd && chave > no->chaves[i]) {
        i++;
    }
    
    // Caso 1: A chave está no nó atual
    if (i < no->qtd && chave == no->chaves[i]) {
        if (no->folha) {
            // Caso 1a: Remoção em folha
            for (int j = i; j < no->qtd - 1; j++) {
                no->chaves[j] = no->chaves[j + 1];
            }
            no->qtd--;
            
            // Se o nó ficou vazio, libera a memória
            if (no->qtd == 0) {
                free(no);
                return NULL;
            }
        } else {
            // Caso 1b: Remoção em nó interno
            if (no->filhos[i]->qtd >= 1) {
                // Substitui pelo predecessor
                int pred = encontrarMinimo(no->filhos[i]);
                no->chaves[i] = pred;
                no->filhos[i] = remover(no->filhos[i], pred);
            } else if (no->filhos[i + 1]->qtd >= 1) {
                // Substitui pelo sucessor
                int succ = encontrarMinimo(no->filhos[i + 1]);
                no->chaves[i] = succ;
                no->filhos[i + 1] = remover(no->filhos[i + 1], succ);
            } else {
                // Caso 1c: Fusão necessária
                No* filhoEsq = no->filhos[i];
                No* filhoDir = no->filhos[i + 1];
                
                // Move a chave do pai para o filho esquerdo
                filhoEsq->chaves[1] = no->chaves[i];
                
                // Move a chave do filho direito
                filhoEsq->chaves[2] = filhoDir->chaves[0];
                filhoEsq->qtd = 3;
                
                // Remove a chave do nó atual
                for (int j = i; j < no->qtd - 1; j++) {
                    no->chaves[j] = no->chaves[j + 1];
                    no->filhos[j + 1] = no->filhos[j + 2];
                }
                no->qtd--;
                
                // Libera o filho direito e continua a remoção
                free(filhoDir);
                no->filhos[i] = remover(filhoEsq, chave);
            }
        }
    } else if (!no->folha) {
        // Caso 2: A chave não está no nó atual - busca no filho apropriado
        no->filhos[i] = remover(no->filhos[i], chave);
    }
    
    // Se o nó ficou vazio, libera a memória
    if (no->qtd == 0) {
        free(no);
        return NULL;
    }
    
    return no;
}

/* FUNÇÃO PRINCIPAL */
int main() {
    No* raiz = criarNo(1);  // Cria a árvore (inicialmente vazia)
    
    int opcao, valor;
    while (1) {
        printf("\nMenu Árvore 2-3:\n");
        printf("1. Inserir\n2. Buscar\n3. Exibir Hierárquico\n");
        printf("4. Exibir por Nível\n5. Remover\n6. Sair\nEscolha: ");
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
                    printf("Chave %d não encontrada.\n", valor);
                }
                break;
                
            case 3:
                printf("Exibição Hierárquica:\n");
                percursoEmOrdem(raiz, 0);
                break;
                
            case 4:
                printf("Exibição por Nível:\n");
                percursoPorNivel(raiz);
                break;
                
            case 5:
                printf("Valor a remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
                
            case 6:
                exit(0);
                
            default:
                printf("Opção inválida!\n");
        }
    }
    
    return 0;
}