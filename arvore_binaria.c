#include <stdio.h>
#include <stdlib.h>
/*
    ARVORE BINÁRIA - Uma arvore de elementos que se ramifica a partir de sua raiz inicial, cada raiz pode possuir até dois elementos
    
    PROPRIEDADES:
    -> Os numeros MENORES que a raiz serão posicionados a ESQUERDA dela, e serão uma nova raiz  
    -> Os numeros MAIORES que a raiz serão posicionados a DIREITA dela, e serão uma nova raiz 
    -> Cada raiz é representada por um nó 
    -> Uma arvore binária não pode repetir o mesmo elemento
    -> A cada nova raiz, origina-se uma sub-arvore, podendo ser:
       --> SUB-ARVORE A ESQUERDA - se a raiz da sub-arvore está localizada a ESQUERDA 
       --> SUB-ARVORE A DIREITA  - se a raiz da sub-arvore está localizada a DIREITA
    
    IMPRESSÃO DA ARVORE BINÁRIA:
    1° FORMA                              - raiz -> esquerda -> direita
    2° FORMA (imprime em ordem crescente) - esquerda -> raiz -> direita
*/

//struct para nós
typedef struct no{        
    int conteudo;           //armazena o valor do nó/raiz | conteúdo do nó/raiz
    struct no *esq, *dir;   //armazenam os valores da esquerda da raiz, e da direita, obedecendo as regras acima
}NO;

//struct para a raiz inicial da arvore
typedef struct{           
    NO *raiz; 
    int tam;                 //armazena o tamanho da arvore              
}arvB;

void Inserir(arvB *A, int V);
void InserirEsquerda(arvB *t, NO *a, int v);
void InserirDireita(arvB *t, NO *a, int v);
/*
    Os procedimentos de imprimir, utilizam de chamadas recursivas mais robustas

    RECURSIVIDADE - chamar uma função ou procedimento dentro dele mesmo

    Ao fazer uma chamada recursiva, o computador só executará o que há além dela somente quando essa chamada terminar. Isso se dá devido ao fato de que a maquina, ao realizar a chamada recursiva, cria uma pilha com as chamadas feitas dentro da função/procedimento, onde o topo da pilha é a ultima chamada, e a base a primeira chamada. Quando todas as chamadas recursivas finalizam, o computador vai desempilhando a pilha e executando os codigos além da chamada recursiva. Ou seja, os codigos serão executados do final para o começo

    EXEMPLO:
    Imprimir(int n){
        if(n == 0){
            printf("%d", n);
        }else{
            Imprimir(n - 1);
            printf("%d", n);
        }
    }
    
    Se n for 5, por exemplo, a impressão sera: 0, 1, 2, 3, 4, 5
    Isso por que o printf dentro de else só sera executado quando todas as chamadas recursivas terminarem, isto é, quando n for 0, a partir dai, o computador vai desempilhar os valores e excutar o codigo depois da chamada recursiva, assim, a impressão ocorre do final para o começo

    CHAMADA RECURSIVA DO EXEMPLO ACIMA:
    1° chamada recursiva, construindo a pilha
        5(base) - 4 - 3 - 2 - 1 - 0(topo)
    2° desempilhando a pilha
        executa printf dentro de else e desempilha 0 - executa printf dentro de else e desempilha 1 - executa printf dentro de else e desempilha 2 - executa printf dentro de else e desempilha 3 - executa printf dentro de else e desempilha 4 - executa printf dentro de else e desempilha 5    
*/
void ImprimirArvoreNormal(NO *R);
void ImprimirArvoreOrdenada(NO *R);
int BuscarNo(NO *R, int con);
NO* RemoverNo(arvB *t, NO *R, int rem);
int AlturaArvore(NO *R);

int main(){
    arvB arvore;                    //nó inicial da arvore
    arvore.tam = 0;
    arvore.raiz = NULL;             //a arvore vai estar vazia
    int op, op2, valor, valor2, remover;     //operadores para switch case, o valor que será atribuido ao conteúdo do nó, o valor do nó que se deseja buscar e o valor do nó que se deseja remover
    
    do{
        printf("\n1 - Inserir\n2 - Imprimir\n3 - Buscar\n4 - Remover\n5 - Altura\n6 - Sair\n");
        scanf("%d", &op);

        switch(op){
            case 1:
                printf("\nDigite o valor a ser inserido\n");
                scanf("%d", &valor);
                Inserir(&arvore, valor);
                break;
            case 2:
                printf("\n1 - Impressao normal\n2 - Impressao em ordem\n");
                scanf("%d", &op2);
                switch(op2){
                    case 1:
                        printf("\nArvore impressa:\n");
                        ImprimirArvoreNormal(arvore.raiz);
                        printf("\nTamanho da arvore/quantidades de nos: %d\n", arvore.tam);
                        break;
                    case 2:
                        printf("\nArvore impressa:\n");
                        ImprimirArvoreOrdenada(arvore.raiz);
                        printf("\nTamanho da arvore/quantidades de nos: %d\n", arvore.tam);
                        break;
                    default:
                        printf("\nOperacao invalida\n");
                        break;
                }
                break;
            case 3:
                printf("\nDigite o no a ser buscado:\n");
                scanf("%d", &valor2);
                printf("\nNo buscado: %d\n", BuscarNo(arvore.raiz, valor2));
                break;
            case 4:
                printf("\nDigite o no a ser removido:\n");
                scanf("%d", &remover);
                arvore.raiz = RemoverNo(&arvore, arvore.raiz, remover);
                break;
            case 5:
                printf("\nAltura da arvore: %d\n", AlturaArvore(arvore.raiz));
                break;
            case 6:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("\nOperacao invalida\n");
                break;
        }
    }while(op != 6);

    return 0;
}

//procedimento para inserir um nó/raiz a arvore
void Inserir(arvB *A, int V){
    //se a arvore estiver vazia
    if(A->raiz == NULL){
        NO *novo = (NO*)malloc(sizeof(NO));  //adiciona um novo nó na arvore, a raiz inicial
        novo->conteudo = V;
        novo->esq = NULL;                    // | ESQUERDA(esq) | RAÍZ PRINCIPAL(A->raiz) | DIREITA(dir) |
        novo->dir = NULL;                    // |      NULL     |            V            |     NULL     |
        A->raiz = novo;
        A->tam = A->tam + 1;
    }else{
        //os ifs estão dispostos desta forma para impedir que haja repetição de nós, já que, não uma comparação de igualdade, só as de "menor que" e "maior que"
        //inserir a ESQUERDA da raiz
        if(V < A->raiz->conteudo){
            InserirEsquerda(A, A->raiz, V);
        }
        //inserir a DIREITA da raiz
        if(V > A->raiz->conteudo){
            InserirDireita(A, A->raiz, V);
        }
    }
}

//procedimento para inserir um nó a ESQUERDA da raiz
void InserirEsquerda(arvB *t, NO *a, int v){
    //se não houver nenhum nó a ESQUERDA da raiz inicial | cria um novo nó a ESQUERDA da raiz inicial
    if(a->esq == NULL){
        NO *novo = (NO*)malloc(sizeof(NO));  //adiciona um novo nó na arvore, a raiz inicial
        novo->conteudo = v;                  
        novo->esq = NULL;                    // | ESQUERDA(esq) | NOVO NÓ(novo/a) | DIREITA(dir) |
        novo->dir = NULL;                    // |      NULL     |        v        |     NULL     |
        a->esq = novo;
        t->tam = t->tam + 1;
    }else{
        //os ifs estão dispostos desta forma para impedir que haja repetição de nós, já que, não uma comparação de igualdade, só as de "menor que" e "maior que"
        //inserir a ESQUERDA de outro nó a ESQUERDA 
        if(v < a->esq->conteudo){
            InserirEsquerda(t, a->esq, v);
        }
        //inserir a DIREITA de um nó a ESQUERDA 
        if(v > a->esq->conteudo){
            InserirDireita(t, a->esq, v);
        }
    }
}

//procedimento para inserir um nó a DIREITA da raiz
void InserirDireita(arvB *t, NO *a, int v){
    //se não houver nenhum nó a DIREITA da raiz inicial | cria um novo nó a DIREITA da raiz inicial
    if(a->dir == NULL){
        NO *novo = (NO*)malloc(sizeof(NO));  //adiciona um novo nó na arvore, a raiz inicial
        novo->conteudo = v;                                  
        novo->esq = NULL;                    // | ESQUERDA(esq) | NOVO NÓ(novo/a) | DIREITA(dir) |
        novo->dir = NULL;                    // |      NULL     |        v        |     NULL     |
        a->dir = novo;
        t->tam = t->tam + 1;
    }else{
        //os ifs estão dispostos desta forma para impedir que haja repetição de nós, já que, não uma comparação de igualdade, só as de "menor que" e "maior que"
        //inserir a DIREITA de outro nó a DIREITA
        if(v > a->dir->conteudo){
            InserirDireita(t, a->dir, v);
        }
        //inserir a ESQUERDA de um nó a DIREITA
        if(v < a->dir->conteudo){
            InserirEsquerda(t, a->dir, v);
        }
    }
}

//procedimento para imprimir a arvore binária - 1° FORMA
void ImprimirArvoreNormal(NO *R){
    //se a raiz não for NULL
    if(R){
        //1° FORMA de impressão:
        printf("%d ", R->conteudo);          //imprime a raíz inicial
        ImprimirArvoreNormal(R->esq);        //imprime a(as) raíz(raízes) a ESQUERDA
        ImprimirArvoreNormal(R->dir);        //imprime a(as) raíz(raízes) a DIREITA
    }
}

//procedimento para imprimir a arvore binária - 2° FORMA
void ImprimirArvoreOrdenada(NO *R){
    //se a raiz não for NULL
    if(R){        
        //2° FORMA de impressão:
        ImprimirArvoreOrdenada(R->esq);      //imprime a(as) raíz(raízes) a ESQUERDA
        printf("%d ", R->conteudo);          //imprime a raíz inicial
        ImprimirArvoreOrdenada(R->dir);      //imprime a(as) raíz(raízes) a DIREITA
    }
}

//função para buscar um nó
int BuscarNo(NO *R, int con){
    if(R == NULL){
        return -1;                           //Retorno um número inteiro negativo, pois a arvore é composta por numeros inteiros positivos, assim, não há como retornar um valor presente na arvore para caso não haja nenhum nó, acarretando em uma comparação falha no case 3 em int main
    }else{
        //vai chegar um momento, se o valor digitado existe na arvore, que o conteudo da raiz analizada vai ser igual ao valor digitado 
        if(R->conteudo == con){
            return R->conteudo;
        }else{
            //se o nó buscado é MENOR que R, isto é, se está a ESQUERDA de R
            if(con < R->conteudo){
                return BuscarNo(R->esq, con); //devido a recursividade, ao chegar nessa linha, a função vai se repetir, mas R vai ser igual a raiz a ESQUERDA da raiz principal ou outra raiz a ESQUERDA
            //se o nó buscado é MAIOR ou IGUAL a R, isto é, se está a DIREITA de R
            }else{
                return BuscarNo(R->dir, con); //devido a recursividade, ao chegar nessa linha, a função vai se repetir, mas R vai ser igual a raiz a DIREITA da raiz principal ou outra raiz a DIREITA
            }
        }
    }
}

//função para remover nó
NO* RemoverNo(arvB *t, NO *R, int rem){
    if(R == NULL){
        printf("\nNo nao encontrado\n");
        return NULL;
    }else{
        //a função vai se repetir até R->conteudo == rem ou se o nó não existir na arvore
        if(R->conteudo == rem){
            //removendo folhas, aqueles nós que não possuem filhos
            if(R->esq == NULL && R->dir == NULL){
                t->tam = t->tam - 1;
                free(R);                         //remove nó folha da memória
                return NULL;                     //o nó a ESQUERDA ou a DIREITA do pai do nó removido vai ser NULL, assim, apagando o nó removido de vez, e tirando um filho do nó pai, o nó anterior do apagado 
            }else{
                //removendo nós com apenas 1 filho | o nó filho substituirá o pai
                if(R->esq == NULL || R->dir == NULL){
                    NO *aux;                      //ponteiro que armazenará o nó filho, que substituirá o pai 
                    //se o filho estiver a ESQUERDA
                    if(R->esq != NULL){
                        aux = R->esq;
                    //se o filho estiver a DIREITA
                    }else{
                        aux = R->dir;
                    }
                    t->tam = t->tam - 1;
                    free(R);
                    return aux;                   //dessa forma, vai retornar o nó filho que substituirá o pai
                //removendo nós com dois filhos | acessar a esquerda do nó que quer remover, depois acessar os nós a direita até chegar em um nó folha, o nó folha substituirá o nó removido
                }else{
                    NO *aux = R->esq;                //para acessar o nó da esquerda, para depois acessar os da esquerda
                    //vai navegar pela direita até chegar em um nó folha
                    while(aux->dir != NULL){
                        aux = aux->dir;
                    }
                    //vamos trocar os conteudos do nó folha e do nó removido, depois, removeremos o nó folha com o valor do nó removido
                    R->conteudo = aux->conteudo;
                    aux->conteudo = rem;
                    R->esq = RemoverNo(t, R->esq, rem); //remover o nó folha com o valor do nó removido | sempre retornará NULL, pois sempre vai ser rem < R->conteudo
                    return R;
                }
            }
        }else{
            if(rem < R->conteudo){
                R->esq = RemoverNo(t, R->esq, rem); //vai receber os valores de retorno acima, ao receber, esse valor será atribuido ou a ESQUERDA ou a DIREITA do nó pai do nó da ultima chamada recursiva, que é o nó removido
            }else{
                R->dir = RemoverNo(t, R->dir, rem); //vai receber os valores de retorno acima, ao receber, esse valor será atribuido ou a ESQUERDA, ou a DIREITA do nó pai do nó da ultima chamada recursiva, que é o nó removido
            }
            return R;                            //após um dos dois acima receberem os valores de retorno, finalizando a chamada recursiva, vai retornar o nó pai do removido
        }
    }
}

//função para calcular altura da arvore
//A altura da arvore diz respeito a quantidade de caminhos percorridos do nó raiz até o ultimo nó folha
int AlturaArvore(NO *R){
    if(R == NULL){
        return -1;
    }else{  
        int esquerda = AlturaArvore(R->esq); // vai percorrer as sub-arvores à esquerda até chegar a NULL
        int direita = AlturaArvore(R->dir);  // vai percorrer as sub-arvores à direita até chegar a NULL
        if(esquerda > direita){
            return esquerda + 1;             // +1 pois, quando a arvore for NULL, o retorno é -1
        }else{
            return direita + 1;              // +1 pois, quando a arvore for NULL, o retorno é -1
        }
    }
}