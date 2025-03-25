//código de algoritmos de ordenação: COUTING SORT!

//Algoritmo de ordenação:é um método que organiza os elementos de uma lista em uma sequência específica, geralmente em ordem crescente ou decrescente, através de comparações e trocas de posições
//Counting Sort: Tipo de algoritmo de ordenação que conta a frequência de cada elemento do array e usa pra dispor ele da forma desejada (NÃO COMPARATIVO)
//Complexidade: BigO(n+k)

#include <stdio.h>
#include <stdlib.h>
//função que vai criar nosso array a ser ordenado
void countSort(int Array[], int T) {
    int max = Array[0]; 
    for (int i = 1; i < T; i++) {
        if (Array[i] > max) {
            max = Array[i];
        }//determina valor maximo do array original
    }

    //Ponteiro que aloca a memória de forma dinamica pro countSort
    int *count = (int *)calloc(max + 1, sizeof(int));
    /*
    max + 1 vai definir o tamanho do array temporário de contagem
    usamos calloc para zerar memoria alocada, importante para o countSort ordernar de forma correta
    */

    for (int i = 0; i < T; i++) {
        count[Array[i]]++;
    }//contagem de ocorrencia de cada elemento, incrementando os valores em seus indices correspondentes
    

    int indice = 0;
    for (int i = 0; i <= max; i++) {
        while (count[i] > 0) {
            Array[indice++] = i;
            count[i]--;
        }
    }//reconstrói array original, só que ordenado a partir do array auxiliar
    free(count);
}

void impArray(int Array[], int T) {
    for (int i = 0; i < T; i++) {
        printf("%d ", Array[i]);
    }
    printf("\n");
}//imprime o array ordenado

int main() {
    int inputArray[] = {4, 2, 2, 8, 3, 3, 1};
    int N = sizeof(inputArray) / sizeof(inputArray[0]);
    
    printf("Array original: ");
    impArray(inputArray, N);
    
    countSort(inputArray, N);
    
    printf("Array ordenado: ");
    impArray(inputArray, N);
    
    return 0;
}
