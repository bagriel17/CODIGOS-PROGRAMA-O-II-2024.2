#include <stdio.h>
#include <stdlib.h>

/*
ARVORE AVL
GRUPO: GABRIEL MELO, MIGUEL PERES, PAULO ANDRE

A arvore AVL, nada mais é que uma arvore binária balanceada, pois, na binária pode ocorrer uma degeneração, que é o desbalanceamento, que torna nossa arvore lenta para buscas.

Mediante rotações (para esquerda ou para direita), que levam em conta o tamanho de cada sub-arvore (nós raizes), a AVL balanceia a arvore.

Para uma arvore ser AVL, o fator de balanceamento (a altura da sub-arvore a esquerda da raiz principal - a altura da sub-arvore a direta da raiz principal) deve ser igual a: 1, 0, -1, diferente disso, a arvore está desbalanceada.

-1: o nó folha mais distante está na sub-arvore a direita da raiz principal/inicial
 1: o nó folha mais distante está na sub-arvore a esquerda da raiz principal/inicial
*/

//struct para criar um nó
typedef struct no{
    int cnt;               //conteudo
    struct no *esq, *dir;  //esquerda e direita da raiz
    short alt;             //altura do nó, short ocupa menos memória | short usa 2 bytes e int usa 4 bytes
}No;

//Função para criar um novo nó
No* NovoNo(int v){
    No *novo = (No*)malloc(sizeof(No));
    if(novo){
        novo->cnt = v;
        novo->esq = NULL; 
        novo->dir = NULL;
        novo->alt = 0;
    }else
        printf("\n\tErro ao alocar memoria\n");
    return novo;
}

//Compara o tamanho de dois nós, e retorna o maior deles | Auxilia no calculo da altura de um nó, ja que, a altura de um nó é a maior altura entre suas sub-arvores + 1
short MaiorNo(short altA, short altB){
    return (altA > altB)? altA: altB;        //operador ternario, a é maior que b? Se sim, retorne a, se não, retorne b
}

//Função que retorna o tamanho de um nó | Auxilia na função FatorDeBalanceamento
short AlturaNo(No *no){
    if(no == NULL)
        return -1;
    else    
        return no->alt;
}

//Função que calcula o fator de balanceamento, para sabermos se a arvore esta balanceada ou não
short FatorDeBalanceamento(No *no){
    if(no)
        return (AlturaNo(no->esq) - AlturaNo(no->dir));
    else
        return 0;
}

//Função para rotacionar a esquerda:
No* RotacaoEsquerda(No *raiz){
    No *FilhoR = raiz->dir;     //filho da raiz à direita
    No *FilhoFR = FilhoR->esq;  //filho à esquerda do filho da raiz

    FilhoR->esq = raiz;         //raiz vai para a esquerda de do filho
    raiz->dir = FilhoFR;        //o FilhoFR vai para a direita da raiz

    //recalculando as alturas:
    raiz->alt = MaiorNo(AlturaNo(raiz->esq), AlturaNo(raiz->dir)) + 1;
    FilhoR->alt = MaiorNo(AlturaNo(FilhoR->esq), AlturaNo(FilhoR->dir)) + 1;
    
    return FilhoR; //a nova raiz
    /*
    ex:
        10                                  
      (raiz)                                     25
          \                                     /  \
           25       ===Balanceamento==>>      10    32
        (FilhoR)                                \
          /  \                                   20
        20    32
     (FilhoFR)          
    */
}

//Função para rotacionar a direita:
No* RotacaoDireita(No *raiz){
    No *FilhoR = raiz->esq;     //filho da raiz à esquerda
    No *FilhoFR = FilhoR->dir;  //filho à direita do filho da raiz

    FilhoR->dir = raiz;         //raiz vai para a direita do filho
    raiz->esq = FilhoFR;        //o FilhoFR vai para a esquerda da raiz

    //recalculando as alturas:
    raiz->alt = MaiorNo(AlturaNo(raiz->esq), AlturaNo(raiz->dir)) + 1;
    FilhoR->alt = MaiorNo(AlturaNo(FilhoR->esq), AlturaNo(FilhoR->dir)) + 1;
    
    return FilhoR; //a nova raiz
    /*
    ex:
       
           50 
         (raiz)                                   35
         /                                       /  \
        35           ==BALANCEAMENTO==>>       20    50
     (FilhoR)                                       /  
     /      \                                     40
    20       40
          (FilhoFR)   
    */
}

//ROTAÇÕES DUPLAS:
//Função para rotação direita esquerda:
No* RotacaoDirEsq(No *raiz){
    raiz->dir = RotacaoDireita(raiz->dir);
    return RotacaoEsquerda(raiz);
    /*
    ex:

        100                                100                                      150
           \                                  \                                    /   \
            200    ==Rotação Direita==>>       150     ==Rotação Esquerda==>>   100     200
           /                                      \  
        150                                        200
    */
}

//Função para rotação esquerda direita:
No* RotacaoEsqDir(No *raiz){
    raiz->esq = RotacaoEsquerda(raiz->esq);
    return RotacaoDireita(raiz);
    /*
    ex:

        100                                 100                                 75         
       /                                   /                                   /  \  
     50       ==Rotação Esquerda==>>     75        ==Rotação Direita==>>     50    100
       \                                /  
        75                            50                
    */
}

//Função que irá balancear a arvore:
No* Balanceamento(No *raiz){
    short FB = FatorDeBalanceamento(raiz);  //Fator de balanceamento DA RAIZ

    /*
    Caso, por exemplo:
    FB < -1 && fb(raiz->dir) == -1           fb(raiz->dir) == 0
                100                               100
                   \                                 \
                    200               &&              200               
                       \                             /   \
                        250                       150     250
    */
    if(FB < -1 && FatorDeBalanceamento(raiz->dir) <= 0)
        raiz = RotacaoEsquerda(raiz);
    //mesma situação, só que oposto do exemplo acima
    else if(FB > 1 && FatorDeBalanceamento(raiz->esq) >= 0)
        raiz = RotacaoDireita(raiz);
    /*
    Caso, por exemplo:
    FB > 1 && fb(raiz->esq) == -1
                100
               /
             50
               \
                75
    */
    else if(FB > 1 && FatorDeBalanceamento(raiz->esq) < 0)
        raiz = RotacaoEsqDir(raiz);
    //oposto do de cima
    else if(FB < 1 && FatorDeBalanceamento(raiz->dir) > 0)
        raiz = RotacaoDirEsq(raiz);
    return raiz;
}

//Função para inserir novo nó:
No* InserirNo(No *raiz, int V){
    if(raiz == NULL)
        raiz = NovoNo(V);
    else if(V < raiz->cnt)
        raiz->esq = InserirNo(raiz->esq, V);
    else if(V > raiz->cnt)
        raiz->dir = InserirNo(raiz->dir, V);
    else    
        printf("\n\tValor invalido\n");       //se o valor digitado já estiver na arvore, não podemos repetir valores
    
    //Devido a inserção, a raiz principal muda de altura, na declaração abaixo recalculamos a altura da raiz
    raiz->alt = MaiorNo(AlturaNo(raiz->esq), AlturaNo(raiz->dir)) + 1;
    //aqui será realizado o balanceamento, que retornará a nova raiz principal para int main
    return Balanceamento(raiz);
}

No* RemoverNo(No *raiz, int chave){
    if(raiz == NULL){
        printf("\n\tValor nao encontrado\n");
        return NULL;
    }else if(raiz->cnt == chave){
        //se for um no folha
        if(raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            printf("\n\tNo removido: %d\n", chave);
            return NULL;
        }else if(raiz->esq == NULL || raiz->dir == NULL){
            No *aux;

            if(raiz->esq != NULL)
                aux = raiz->esq;
            else
                aux = raiz->dir;
            free(raiz);
            printf("\n\tNo removido: %d\n", chave);
            return aux;
        }else{
            No *aux = raiz->esq;

            while(aux->dir != NULL)
                aux = aux->dir;
            raiz->cnt = aux->cnt;
            aux->cnt = chave;
            raiz->esq = RemoverNo(raiz->esq, chave);
            return raiz;
        }
    }else if(chave < raiz->cnt)
        raiz->esq = RemoverNo(raiz->esq, chave);
    else
        raiz->dir = RemoverNo(raiz->dir, chave);

    //mesmos conceitos apresentados na inserção se enquadram aqui
    raiz->alt = MaiorNo(AlturaNo(raiz->esq), AlturaNo(raiz->dir)) + 1;
    return Balanceamento(raiz);
} 

//procedimento para imprimir a arvore
void ImprimirAVL(No *raiz, int nivel){
    if(raiz){
        ImprimirAVL(raiz->dir, nivel + 1);   //o nivel serve para tabular ao chegar em NULL, de modo a imprimir a arvore como ela é geralmente ilustrada
        printf("\n\n");

        for(int i = 0; i < nivel; i++)         
            printf("\t");                    //tabulação, um tab no terminal
        printf("%d", raiz->cnt);

        ImprimirAVL(raiz->esq, nivel + 1);
    }
}

int main(){
    int op, valor;
    No *raiz = NULL;

    do{
        printf("\n\n\t0 - SAIR\n\t1 - INSERIR\n\t2 - REMOVER\n\t3 - IMPRIMIR\n");
        scanf("%d", &op);

        switch(op){
            case 0:
                printf("\n\tEncerrando...\n");
                break;
            case 1:
                printf("\n\tValor a INSERIR: ");
                scanf("%d", &valor);

                raiz = InserirNo(raiz, valor);
                break;
            case 2:
                printf("\n\tValor a REMOVER: ");
                scanf("%d", &valor);

                raiz = RemoverNo(raiz, valor);
                break;
            case 3:
                ImprimirAVL(raiz, 1);
                break;
            default:
                printf("\n\tOperador invalido\n");
                break;
        }
    }while(op != 0);

    return 0;
}