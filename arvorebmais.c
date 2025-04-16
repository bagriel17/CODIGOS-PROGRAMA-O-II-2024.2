//ARVORE B+
//GRUPO: MIGUEL PERES; GABRIEL MELO; PAULO ANDRE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> // para toupper()

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

// Protótipos de funções
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
int EncontrarPosicaoChave(No *no, int chave);
int EncontrarPredecessor(No *no, int pos);
int EncontrarSucessor(No *no, int pos);
void Remover(int chave);
void RemoverDaFolha(No *folha, int chave);
void EmprestarOuMesclar(No *no);
void MesclarFolhas(No *folha, No *vizinho, int chavePai);
void MesclarInternos(No *no, No *vizinho, int chavePai);
void menu();

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

// procedimento para remover uma chave da folha
void RemoverDaFolha(No *folha, int chave) {
    int i = 0;
    while (i < folha->num_chaves && folha->chaves[i] != chave) {
        i++;
    }
    
    if (i == folha->num_chaves) {
        printf("Chave %d não encontrada na folha.\n", chave);
        return;
    }
    
    // Deslocar as chaves para preencher o espaço
    for (int j = i; j < folha->num_chaves - 1; j++) {
        folha->chaves[j] = folha->chaves[j + 1];
    }
    
    folha->num_chaves--;
}

// Função para mesclar folhas
void MesclarFolhas(No *folha, No *vizinho, int chavePai) {
    // Copiar chaves do vizinho para a folha
    for (int i = 0; i < vizinho->num_chaves; i++) {
        folha->chaves[folha->num_chaves + i] = vizinho->chaves[i];
    }
    
    folha->num_chaves += vizinho->num_chaves;
    folha->prox = vizinho->prox;
    
    // Remover a chave do pai e o ponteiro para o vizinho
    No *pai = folha->pai;
    int pos = 0;
    while (pos < pai->num_chaves && pai->chaves[pos] < chavePai) {
        pos++;
    }
    
    // Deslocar chaves e ponteiros no pai
    for (int i = pos; i < pai->num_chaves - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
    }
    for (int i = pos + 1; i < pai->num_chaves; i++) {
        pai->filho[i] = pai->filho[i + 1];
    }
    
    pai->num_chaves--;
    
    free(vizinho);
}

// Função para mesclar nós internos
void MesclarInternos(No *no, No *vizinho, int chavePai) {
    // Adicionar a chave do pai ao nó
    no->chaves[no->num_chaves] = chavePai;
    no->num_chaves++;
    
    // Copiar chaves e ponteiros do vizinho
    for (int i = 0; i < vizinho->num_chaves; i++) {
        no->chaves[no->num_chaves + i] = vizinho->chaves[i];
    }
    for (int i = 0; i <= vizinho->num_chaves; i++) {
        no->filho[no->num_chaves + i] = vizinho->filho[i];
        vizinho->filho[i]->pai = no;
    }
    
    no->num_chaves += vizinho->num_chaves;
    
    // Remover a chave do pai e o ponteiro para o vizinho
    No *pai = no->pai;
    int pos = 0;
    while (pos < pai->num_chaves && pai->chaves[pos] < chavePai) {
        pos++;
    }
    
    // Deslocar chaves e ponteiros no pai
    for (int i = pos; i < pai->num_chaves - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
    }
    for (int i = pos + 1; i < pai->num_chaves; i++) {
        pai->filho[i] = pai->filho[i + 1];
    }
    
    pai->num_chaves--;
    
    free(vizinho);
}

// Função para emprestar chave de um vizinho ou mesclar
void EmprestarOuMesclar(No *no) {
    if (no == raiz) {
        if (no->num_chaves == 0) {
            if (no->e_folha) {
                free(no);
                raiz = NULL;
            } else {
                raiz = no->filho[0];
                raiz->pai = NULL;
                free(no);
            }
        }
        return;
    }
    
    No *pai = no->pai;
    int pos = 0;
    while (pos <= pai->num_chaves && pai->filho[pos] != no) {
        pos++;
    }
    
    // Tentar emprestar do irmão esquerdo
    if (pos > 0) {
        No *vizinhoEsq = pai->filho[pos - 1];
        if (vizinhoEsq->num_chaves > MIN_CHAVES) {
            // Emprestar do vizinho esquerdo
            if (no->e_folha) {
                // Mover a última chave do vizinho para o nó
                for (int i = no->num_chaves; i > 0; i--) {
                    no->chaves[i] = no->chaves[i - 1];
                }
                no->chaves[0] = vizinhoEsq->chaves[vizinhoEsq->num_chaves - 1];
                no->num_chaves++;
                vizinhoEsq->num_chaves--;
                
                // Atualizar a chave do pai
                pai->chaves[pos - 1] = no->chaves[0];
            } else {
                // Mover a chave do pai para o nó
                no->filho[no->num_chaves + 1] = no->filho[no->num_chaves];
                for (int i = no->num_chaves; i > 0; i--) {
                    no->chaves[i] = no->chaves[i - 1];
                    no->filho[i] = no->filho[i - 1];
                }
                no->chaves[0] = pai->chaves[pos - 1];
                no->filho[0] = vizinhoEsq->filho[vizinhoEsq->num_chaves];
                no->filho[0]->pai = no;
                no->num_chaves++;
                
                // Mover a última chave do vizinho para o pai
                pai->chaves[pos - 1] = vizinhoEsq->chaves[vizinhoEsq->num_chaves - 1];
                
                vizinhoEsq->num_chaves--;
            }
            return;
        }
    }
    
    // Tentar emprestar do irmão direito
    if (pos < pai->num_chaves) {
        No *vizinhoDir = pai->filho[pos + 1];
        if (vizinhoDir->num_chaves > MIN_CHAVES) {
            // Emprestar do vizinho direito
            if (no->e_folha) {
                // Mover a primeira chave do vizinho para o nó
                no->chaves[no->num_chaves] = vizinhoDir->chaves[0];
                no->num_chaves++;
                
                // Deslocar as chaves do vizinho
                for (int i = 0; i < vizinhoDir->num_chaves - 1; i++) {
                    vizinhoDir->chaves[i] = vizinhoDir->chaves[i + 1];
                }
                vizinhoDir->num_chaves--;
                
                // Atualizar a chave do pai
                pai->chaves[pos] = vizinhoDir->chaves[0];
            } else {
                // Mover a chave do pai para o nó
                no->chaves[no->num_chaves] = pai->chaves[pos];
                no->filho[no->num_chaves + 1] = vizinhoDir->filho[0];
                no->filho[no->num_chaves + 1]->pai = no;
                no->num_chaves++;
                
                // Mover a primeira chave do vizinho para o pai
                pai->chaves[pos] = vizinhoDir->chaves[0];
                
                // Deslocar chaves e ponteiros do vizinho
                for (int i = 0; i < vizinhoDir->num_chaves - 1; i++) {
                    vizinhoDir->chaves[i] = vizinhoDir->chaves[i + 1];
                }
                for (int i = 0; i < vizinhoDir->num_chaves; i++) {
                    vizinhoDir->filho[i] = vizinhoDir->filho[i + 1];
                }
                vizinhoDir->num_chaves--;
            }
            return;
        }
    }
    
    // Se não puder emprestar, mesclar
    if (pos > 0) {
        // Mesclar com o vizinho esquerdo
        No *vizinhoEsq = pai->filho[pos - 1];
        if (no->e_folha) {
            MesclarFolhas(vizinhoEsq, no, pai->chaves[pos - 1]);
        } else {
            MesclarInternos(vizinhoEsq, no, pai->chaves[pos - 1]);
        }
        no = vizinhoEsq;
    } else {
        // Mesclar com o vizinho direito
        No *vizinhoDir = pai->filho[pos + 1];
        if (no->e_folha) {
            MesclarFolhas(no, vizinhoDir, pai->chaves[pos]);
        } else {
            MesclarInternos(no, vizinhoDir, pai->chaves[pos]);
        }
    }
    
    // Verificar se o pai ficou com poucas chaves
    if (pai->num_chaves < MIN_CHAVES) {
        EmprestarOuMesclar(pai);
    }
}

// Adicionando esta função auxiliar para encontrar a chave no nó interno
int EncontrarPosicaoChave(No *no, int chave) {
  int pos = 0;
  while (pos < no->num_chaves && chave > no->chaves[pos]) {
      pos++;
  }
  return pos;
}

// Função para encontrar o predecessor de uma chave em um nó interno
int EncontrarPredecessor(No *no, int pos) {
  No *atual = no->filho[pos];
  while (!atual->e_folha) {
      atual = atual->filho[atual->num_chaves];
  }
  return atual->chaves[atual->num_chaves - 1];
}

// Função para encontrar o sucessor de uma chave em um nó interno
int EncontrarSucessor(No *no, int pos) {
  No *atual = no->filho[pos + 1];
  while (!atual->e_folha) {
      atual = atual->filho[0];
  }
  return atual->chaves[0];
}

// Função principal para remover uma chave
void Remover(int chave) {
  if (raiz == NULL) {
      printf("Árvore vazia.\n");
      return;
  }

  No *folha = LocalizaFolha(raiz, chave);
  int pos_folha = -1;
  
  // Verificar se a chave existe na folha
  for (int i = 0; i < folha->num_chaves; i++) {
      if (folha->chaves[i] == chave) {
          pos_folha = i;
          break;
      }
  }

  if (pos_folha == -1) {
      printf("Chave %d não encontrada na árvore.\n", chave);
      return;
  }

  // Remover da folha
  RemoverDaFolha(folha, chave);

  // Se a folha é a raiz, não precisa fazer mais nada
  if (folha == raiz) {
      if (folha->num_chaves == 0) {
          free(folha);
          raiz = NULL;
      }
      return;
  }

  // Se a folha ficou com poucas chaves, tratar underflow
  if (folha->num_chaves < MIN_CHAVES) {
      EmprestarOuMesclar(folha);
  }

  // Agora precisamos remover a chave dos nós internos se necessário
  No *no = folha;
  while (no->pai != NULL) {
      No *pai = no->pai;
      int pos_pai = 0;
      
      // Encontrar a posição do filho no pai
      while (pos_pai <= pai->num_chaves && pai->filho[pos_pai] != no) {
          pos_pai++;
      }

      // Se a chave removida era a primeira da folha, precisamos atualizar os nós internos
      if (pos_folha == 0 && pos_pai > 0) {
          // A chave no pai que aponta para esta folha pode precisar ser atualizada
          if (no->num_chaves > 0) {
              pai->chaves[pos_pai - 1] = no->chaves[0];
          } else if (no->prox != NULL) {
              pai->chaves[pos_pai - 1] = no->prox->chaves[0];
          }
      }

      no = pai;
  }

  printf("Chave %d removida com sucesso.\n", chave);
}

// Função para remover uma chave de um nó interno
void RemoverDoInterno(No *no, int chave) {
  int pos = EncontrarPosicaoChave(no, chave);
  
  if (pos < no->num_chaves && no->chaves[pos] == chave) {
      // Caso 1: A chave está neste nó interno
      if (no->filho[pos]->num_chaves > MIN_CHAVES) {
          // Substituir pelo predecessor
          int pred = EncontrarPredecessor(no, pos);
          no->chaves[pos] = pred;
          // Remover o predecessor da folha
          No *folha = LocalizaFolha(no->filho[pos], pred);
          RemoverDaFolha(folha, pred);
          if (folha->num_chaves < MIN_CHAVES) {
              EmprestarOuMesclar(folha);
          }
      } else if (no->filho[pos + 1]->num_chaves > MIN_CHAVES) {
          // Substituir pelo sucessor
          int succ = EncontrarSucessor(no, pos);
          no->chaves[pos] = succ;
          // Remover o sucessor da folha
          No *folha = LocalizaFolha(no->filho[pos + 1], succ);
          RemoverDaFolha(folha, succ);
          if (folha->num_chaves < MIN_CHAVES) {
              EmprestarOuMesclar(folha);
          }
      } else {
          // Mesclar os filhos e remover a chave
          No *esq = no->filho[pos];
          No *dir = no->filho[pos + 1];
          
          if (esq->e_folha) {
              MesclarFolhas(esq, dir, chave);
          } else {
              MesclarInternos(esq, dir, chave);
          }
          
          // Remover a chave e o ponteiro do nó
          for (int i = pos; i < no->num_chaves - 1; i++) {
              no->chaves[i] = no->chaves[i + 1];
          }
          for (int i = pos + 1; i < no->num_chaves; i++) {
              no->filho[i] = no->filho[i + 1];
          }
          no->num_chaves--;
          
          if (no->num_chaves < MIN_CHAVES) {
              if (no == raiz) {
                  if (no->num_chaves == 0) {
                      raiz = no->filho[0];
                      raiz->pai = NULL;
                      free(no);
                  }
              } else {
                  EmprestarOuMesclar(no);
              }
          }
      }
  } else {
      // A chave não está neste nó, continuar descendo
      No *filho = no->filho[pos];
      if (filho->num_chaves == MIN_CHAVES) {
          // Garantir que o filho tem chaves suficientes antes de descer
          EmprestarOuMesclar(filho);
          // Pode ter mudado a estrutura, precisamos reavaliar
          RemoverDoInterno(no, chave);
      } else {
          RemoverDoInterno(filho, chave);
      }
  }
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

// Menu interativo
void menu() {
    char opcao;
    int chave;
    
    do {
        printf("\n--- Menu Árvore B+ ---\n");
        printf("1. Inserir chave\n");
        printf("2. Remover chave\n");
        printf("3. Buscar chave\n");
        printf("4. Imprimir árvore\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &opcao);
        
        switch(opcao) {
            case '1':
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                Inserir(chave);
                break;
            case '2':
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                Remover(chave);
                break;
            case '3':
                printf("Digite a chave a ser buscada: ");
                scanf("%d", &chave);
                BuscaChave(chave);
                break;
            case '4':
                printf("\nEstrutura da Árvore B+ (BFS por níveis):\n");
                ImprimirBFS(raiz);
                break;
            case '5':
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != '5');
}

int main() {
    menu();
    return 0;
}