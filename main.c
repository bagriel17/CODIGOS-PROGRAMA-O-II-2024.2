//código de algoritmos de ordenação: COUTING SORT!

//Algoritmo de ordenação:é um método que organiza os elementos de uma lista em uma sequência específica, geralmente em ordem crescente ou decrescente, através de comparações e trocas de posições
//Counting Sort: Tipo de algoritmo de ordenação que conta a frequência de cada elemento do aray e usa pra dispor ele da forma desejada (NÃO COMPARATIVO)
//Complexidade: 

#include <stdio.h>
#include <stdlib.h>

void countSort(int Array[], int T) {
    int max = Array[0];
    for (int i = 1; i < T; i++) {
        if (Array[i] > max) {
            max = Array[i];
        }
    }

    int *count = (int *)calloc(max + 1, sizeof(int));

    for (int i = 0; i < T; i++) {
        count[Array[i]]++;
    }

    int indice = 0;
    for (int i = 0; i <= max; i++) {
        while (count[i] > 0) {
            Array[indice++] = i;
            count[i]--;
        }
    }
    free(count);
}

void impArray(int Array[], int T) {
    for (int i = 0; i < T; i++) {
        printf("%d ", Array[i]);
    }
    printf("\n");
}

int main() {
    int inputArray[] = {4, 3, 12, 1, 5, 5, 3, 9};
    int N = sizeof(inputArray) / sizeof(inputArray[0]);
    
    printf("Array original: ");
    impArray(inputArray, N);
    
    countSort(inputArray, N);
    
    printf("Array ordenado: ");
    impArray(inputArray, N);
    
    return 0;
}