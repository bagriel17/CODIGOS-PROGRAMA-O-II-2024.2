#include <stdio.h>  
#include <stdlib.h>
#include <string.h>  //uso do memcpy

//FUNÇÃO PARA ALOCAR ESPAÇOS NO VETOR:
int* AlocacaoDinamina(int quantidade){
    int *vetor = malloc(sizeof(int) * quantidade);

    if(vetor){    //se alocou corretamente | vetor != NULL
        return vetor;
    }else{    
        printf("\nErro ao alocar memoria\n");
        exit(1);  //encerra o programa
    }
}

//FUNÇÃO PARA EXTRAIR O MAIOR ELEMENTO DO VETOR:
int MaiorElemento(int *vetor, int quantV){
    int maior = vetor[0];
    
    for(int i = 1; i < quantV; i++){
        if(vetor[i] > maior){
            maior = vetor[i];
        }
    }
    
    return maior;
}

//PROCEDIMENTO PARA REALIZAR O COUNTING SORT COM OS DIGITOS EXTRAIDOS:
void CountingSort(int *vetor, int quantV, int div, int base, int *Vtemp){
    int aux, acum = 0;   
    int C[base];                        //vetor que conta a frequência que cada dígito dos numeros do vetor original se repete
    memset(C, 0, sizeof(int) * base);   //prenchendo C com zeros

    #define DIGITO(X) (X / div) % base  //fórmula para separar digito de um numero

    //computando frequência dos dígitos:
    for(int i = 0; i < quantV; i++){
        C[DIGITO(vetor[i])]++;
    }
    
    //soma de prefixos:
    for(int i = 0; i < base; i++){
        aux = C[i];
        C[i] = acum;
        acum += aux;
    }
    
    //ordenando pelo digito menos significativo | preenchendo Vtemp:
    for(int i = 0; i < quantV; i++){
        Vtemp[C[DIGITO(vetor[i])]] = vetor[i];
        C[DIGITO(vetor[i])]++;
    }

    memcpy(vetor, Vtemp, sizeof(int) * quantV);  //copia Vtemp para o vetor original
}

//PROCEDIMENTO PARA RADIX SORT:
void RadixSort(int *vetor, int quantV){
    int k = MaiorElemento(vetor, quantV);   //maior elemento do vetor | determina quantos counting sort fazer
    int div = 1;                            //posição do digito
    int *Vtemp = AlocacaoDinamina(quantV);  //criando uma copia do vetor

    while(k > 0){
        CountingSort(vetor, quantV, div, 10, Vtemp); 
        div *= 10;  //para separar os dígitos dos numeros do vetor, do menos para o mais significativo
        k /= 10;    //subtrai o digito menos significativo de k
    }

    free(Vtemp);    //libera o vetor copia
}

//PROCEDIMENTO PARA IMPRIMIR O VETOR:
void ImprimirVetor(int *vetor, int quantV){
    for(int i = 0; i < quantV; i++){
        printf("%d ", vetor[i]);
    }
}

int main(){
    int *VETOR;  //vetor dinâmico
    int QUANTV;  //quantidade de elementos
    int op = 1;  //operador da repetição

    do{
        printf("\nInforme a quantidade de numeros a ordenar:\n");
        scanf("%d", &QUANTV);
    
        VETOR = AlocacaoDinamina(QUANTV);  //Aloca espaços ao vetor de acordo com o valor digitado pelo usuário
    
        printf("\nInsira os valores a ordenar:\n");
        for(int i = 0; i < QUANTV; i++){
            scanf("%d", &VETOR[i]);        //preenchendo vetor
        }

        printf("\nDesordenado => ");
        ImprimirVetor(VETOR, QUANTV);      //impressão desordenada
        
        RadixSort(VETOR, QUANTV);          //efetuando o radix com os elementos do vetor
        
        printf("\nOrdenado => ");
        ImprimirVetor(VETOR, QUANTV);      //impressão ordenada
        
        //menu de opções para repetir operação ou não:
        printf("\n\n\tQUALQUER TECLA - Repetir\n                0 - Sair\n");  
        scanf("%d", &op);

        free(VETOR);                       //libera o vetor da memória e efetua mais uma operação 
    }while(op != 0);

    printf("\nEncerrando...\n");           //mensagem de fim da execução do programa
    return 0;
}