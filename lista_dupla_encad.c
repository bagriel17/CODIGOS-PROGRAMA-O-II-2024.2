/*
APLICANDO LISTAS EM C - LISTA DUPLAMENTE ENCADEADA

GRUPO: Gabriel Melo, Miguel Peres, Paulo Andre

As listas DUPLAMENTE encadeadas permitem inserir valores em qualquer lugar da lista, no inicio, no meio ou no fim

ENTRETANTO:
Aqui os NÓS se aplicam quase da mesma forma do que nas pilhas e listas simplesmente encadeadas, só que os ponteiros dele, no caso das listas DUPLAMENTE encadeadas, aponta para o nó da frente e o de trás também. O ultimo nó aponta para NULL
*/
#include <stdio.h>
#include <stdlib.h>

//struct para operações com nós
typedef struct no{
    int conteudo;
    struct no *proximo;
    struct no *anterior;
}NO;

void Iinicio(NO **lista, int n);
void Ifinal(NO **lista, int n);
void Imeio(NO **lista, int n, int r);
void Iordenado(NO **lista, int n);
NO *removerP(NO **lista);
NO *removerU(NO **lista);
NO *removerE(NO **lista, int n);
NO *BUSCA(NO **lista, int n);
void exibirL(NO *L);
void exibirLcontrario(NO *L);
NO* ultimoNO(NO **lista);

int main(){
    NO *lista = NULL, *recebido;
    int nlista, nreferencia, operacao, operacao2;

    do{
        printf("\nLISTA DUPLAMENTE ENCADEADA - OPERACOES\n1 - Inserir no INICIO\n2 - Inserir no FINAL\n3 - Inserir no MEIO\n4 - Inserir ORDENADAMENTE\n5 - REMOVER\n6 - BUSCAR valor\n7 - EXIBIR lista\n8 - EXIBIR lista a partir do ULTIMO NO\n9 - SAIR\n");
        scanf("%d", &operacao);

        switch(operacao){
            case 1:
                printf("\nQual o valor deseja inserir: ");
                scanf("%d", &nlista);
                Iinicio(&lista, nlista);
                break;
            case 2:
                printf("\nQual o valor deseja inserir: ");
                scanf("%d", &nlista);
                Ifinal(&lista, nlista);
                break;
            case 3:
                printf("\nQual o valor deseja inserir e a referencia: ");   //a referência é o nó anterior ao nó que queremos inserir
                scanf("%d %d", &nlista, &nreferencia);
                Imeio(&lista, nlista, nreferencia);
                break;
            case 4:
                printf("\nQual o valor deseja inserir: ");
                scanf("%d", &nlista);
                Iordenado(&lista, nlista);
                break;
            case 5:
                printf("\n1 - REMOVER primeiro NO\n2 - REMOVER ultimo NO\n3 - REMOVER NO ESPECIFICO\n");
                scanf("%d", &operacao2);
                switch(operacao2){
                    case 1:
                        recebido = removerP(&lista);
                        if(recebido){
                            printf("\nValor removido: %d\n", recebido->conteudo);
                            free(recebido);
                        }else{
                            printf("\nSem nos a remover ou valor inexistente\n");
                        }
                        break; 
                    case 2:
                        recebido = removerU(&lista);
                        if(recebido){
                            printf("\nValor removido: %d\n", recebido->conteudo);
                            free(recebido);
                        }else{
                            printf("\nSem nos a remover ou valor inexistente\n");
                        }
                        break;
                    case 3:
                        printf("\nQual o valor deseja remover: ");
                        scanf("%d", &nlista);
                        recebido = removerE(&lista, nlista);
                        //se remove != NULL
                        if(recebido){
                            printf("\nValor removido: %d\n", recebido->conteudo);
                            free(recebido);                                         //libera da memória o nó removido
                        }else{
                            printf("\nSem nos a remover ou valor inexistente\n");
                        }
                        break;
                    default:
                        printf("\nValor invalido...\n");
                        break;
                }
                break;
            case 6:
                printf("\nQual o valor deseja BUSCAR: ");
                scanf("%d", &nlista);
                recebido = BUSCA(&lista, nlista);
                if(recebido){
                    printf("\nValor encontrado: %d\n", recebido->conteudo);
                }else{
                    printf("\nValor nao encontrado ou inexistente\n");
                }
                break;
            case 7:
                exibirL(lista);
                break;
            case 8:
                exibirLcontrario((ultimoNO(&lista)));
                break;
            case 9:
                printf("\nEncerrando...");
                break;
            default:
                printf("\nValor invalido...\n");
                break;
        }
    }while(operacao != 9);

    return 0;
}

//void para inserir um novo número no início da lista
void Iinicio(NO **lista, int n){    
    NO *novoNO = malloc(sizeof(NO)); 

    if(novoNO){                      
        novoNO->conteudo = n;        
        novoNO->proximo = *lista;    
        novoNO->anterior = NULL;            // NULL <- novoNO -> *lista || NULL <- novoNO -> NULL (caso a lista esteja vazia)
        //para caso ja tenha um nó na lista
        if(*lista){
            (*lista)->anterior = novoNO;    // NULL <- novoNO -> <- *lista -> NULL/outro nó 
        }
        *lista = novoNO;                    // NULL <- novoNO(nova *lista) -> <- outro nó(antiga *lista)
    }else{
        printf("\nErro ao alocar memoria...\n");
    }
}

//void para inserir um novo número no final da lista
void Ifinal(NO **lista, int n){
    NO *novoNO = malloc(sizeof(NO));
    NO *percorreL;                          

    if(novoNO){
        novoNO->conteudo = n;               
        novoNO->proximo = NULL;

        if(*lista == NULL){
            *lista = novoNO;                        
            novoNO->anterior = NULL;                // NULL <- novoNO(agora, *lista) -> NULL
        }else{
            percorreL = *lista;
            while(percorreL->proximo){
                percorreL = percorreL->proximo;
            }
            percorreL->proximo = novoNO;           // percorreL -> novoNO -> NULL
            novoNO->anterior = percorreL;          // percorreL -> <- novoNO -> NULL
        }
    }else{
        printf("\nErro ao alocar memoria...\n");
    }
}

//void para inserir um novo número no meio da lista
void Imeio(NO **lista, int n, int r){
    NO *novoNO = malloc(sizeof(NO));
    NO *percorreL;  
    
    novoNO->conteudo = n;

    if(novoNO){
        if(*lista == NULL){
            novoNO->proximo = NULL;
            novoNO->anterior = NULL;
            *lista = novoNO;                           //NULL <- novoNO(agora, *lista) -> NULL
        }else{
            percorreL = *lista;
            while(percorreL->conteudo != r && percorreL->proximo){
                percorreL = percorreL->proximo;
            }
            novoNO->proximo = percorreL->proximo;      // novoNO -> Outro nó(percorreL->proximo) 
            if(percorreL->proximo){
                percorreL->proximo->anterior = novoNO;     // novoNO -> <- Outro nó(percorreL->proximo)
            } 
            novoNO->anterior = percorreL;              // percorreL <- novoNO -> <- Outro nó(percorreL->proximo) 
            percorreL->proximo = novoNO;               // percorreL -> <- novoNO(novo percorreL->proximo) -> <- Outro nó(antigo percorreL->proximo)
        }
    }else{
        printf("\nErro ao alocar memoria...\n");
    }
}

//void para ordenar os valores digitados
void Iordenado(NO **lista, int n){
    NO *novoNO = malloc(sizeof(NO));
    NO *percorreL;

    if(novoNO){
        novoNO->conteudo = n;

        if(*lista == NULL){
            novoNO->proximo = NULL;
            novoNO->anterior = NULL;
            *lista = novoNO;
        //if para caso o número inserido seja menor que o primeiro elemento da lista
        }else if(novoNO->conteudo < (*lista)->conteudo){
            novoNO->proximo = *lista;                   // novoNO -> *lista
            (*lista)->anterior = novoNO;                // novoNO -> <- *lista
            *lista = novoNO;                            // novoNO(nova *lista) -> <- Outro nó(antiga *lista)
        //else para caso o número seja maior que um ou + nós da lista, mais seja menor que outro nó na lista
        }else{
            percorreL = *lista;
            while(percorreL->proximo && novoNO->conteudo > percorreL->proximo->conteudo){
                percorreL = percorreL->proximo;
            }
            novoNO->proximo = percorreL->proximo;        // novoNO -> percorreL->proximo
            if(percorreL->proximo){
                percorreL->proximo->anterior = novoNO;       // novoNO -> <- percorreL->proximo
            }
            novoNO->anterior = percorreL;                // percorreL <- novoNO -> <- percorreL->proximo
            percorreL->proximo = novoNO;                 // percorreL -> <- novoNO(novo percorreL->proximo) -> <- percorreL->proximo(antigo)
        }
    }else{
        printf("\nErro ao alocar memoria...\n");
    }
}

//função para remover o primeiro nó
NO *removerP(NO **lista){
    NO *NOremovido = NULL;

    if(*lista){
        NOremovido = *lista;
        *lista = NOremovido->proximo;
        if(*lista){
            (*lista)->anterior = NULL;
        }  
    }
    return NOremovido;
}

//função para remover o ultimo nó
NO *removerU(NO **lista){
    NO *NOremovido = NULL;
    NO *percorreL;

    if(*lista){
        percorreL = *lista;
        while(percorreL->proximo){
            percorreL = percorreL->proximo;
        }
        if(percorreL->anterior){
            percorreL->anterior->proximo = NULL;  
        }else{
            *lista = NULL;                       //lista vazia
        }
        NOremovido = percorreL;
    }
    return NOremovido;
}

//função para remover nós
NO *removerE(NO **lista, int n){
    NO *NOremovido = NULL;                                  //para caso a lista seja nula, ele retorna um valor nulo
    NO *percorreL;

    if(*lista){
        //se deseja remover o primeiro elemento da lista
        if((*lista)->conteudo == n){
            NOremovido = *lista;
            *lista = NOremovido->proximo;                          //NOremovido(antiga *lista) -> NOremovido->proximo(atual *lista)
            //se existir outro nó na lista      
            if(*lista){
                (*lista)->anterior = NULL;                         // NULL <- *lista(NOremovido->proximo) 
            }         
        }else{
            percorreL = *lista;
            while(percorreL->proximo && percorreL->proximo->conteudo != n){
                percorreL = percorreL->proximo;
            }
            if(percorreL->proximo){
                NOremovido = percorreL->proximo;                    // NOremovido(percorreL->proximo)       
                percorreL->proximo = NOremovido->proximo;           // NOremovido(antigo percorreL->proximo) -> NOremovido->proximo(atual percorreL->proximo)
                //precisa checar de novo se percorreL->proximo não é nulo, pois ele precisa apontar para o anterior do nó removido 
                if(percorreL->proximo){
                    percorreL->proximo->anterior = percorreL;        // percorreL -> <- percorreL->proximo
                }  
            }
        }
    }

    return NOremovido;
}

//função para buscar um nó da lista
NO *BUSCA(NO **lista, int n){
    NO *valor = NULL;       
    NO *percorreL = *lista;

    while(percorreL && percorreL->conteudo != n){
        percorreL = percorreL->proximo;
    }
    if(percorreL){
        valor = percorreL;
    }

    return valor;
}

//void para exibir a lista do primeiro Nó para o ultimo
void exibirL(NO *L){
    printf("\n------------LISTA------------\n");
    while(L){
        printf("%d ", L->conteudo);
        L = L->proximo;                //L valerá o nó seguinte, aquele que está em percorreL, ja que percorreL é um ponteiro para lista, ou seja, ele e o ponteiro lista compartilham as mesmas informações, menos a do 1° nó, esse só esta armazenado no ponteiro lista
    }
    printf("\n-----------------------------\n");
}

//void para exibir a lista do ultimo Nó para o primeiro
void exibirLcontrario(NO *L){
    //L vale o ultimo nó
    printf("\n------------LISTA------------\n");
    while(L){
        printf("%d ", L->conteudo);     
        L = L->anterior;                //vai fazer a lista exibir do ultimo nó para o primeiro
    }
    printf("\n-----------------------------\n");
}

//função para passar o ultimo nó para o void exibirLcontrario
NO* ultimoNO(NO **lista){
    NO *percorreL = *lista;

    while(percorreL->proximo){
        percorreL = percorreL->proximo;
    }
    return percorreL;
}