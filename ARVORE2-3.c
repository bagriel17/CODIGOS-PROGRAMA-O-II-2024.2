#include <stdio.h>
#include <stdlib.h>

#define MAX 2

typedef struct No {
    int chaves[MAX];
    struct No *filhos[MAX + 1];
    int qtd, folha;
} No;

No *criarNo(int folha) {
    No *no = malloc(sizeof(No));
    no->qtd = 0;
    no->folha = folha;
    for (int i = 0; i <= MAX; i++) no->filhos[i] = NULL;
    return no;
}

void ordem(No *r) {
    if (!r) return;
    for (int i = 0; i < r->qtd; i++) {
        ordem(r->filhos[i]);
        printf("%d ", r->chaves[i]);
    }
    ordem(r->filhos[r->qtd]);
}

No *inserir(No *raiz, int chave);

int main() {
    No *raiz = NULL;
    int opcao, valor;
    while (1) {
        printf("1. Inserir\n2. Exibir Ordem\n3. Sair\nEscolha: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                ordem(raiz);
                printf("\n");
                break;
            case 3:
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

void inserirEmNo(No *no, int chave, No *filhoDir) {
    int i = no->qtd - 1;
    while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i];
        no->filhos[i + 2] = no->filhos[i + 1];
        i--;
    }
    no->chaves[i + 1] = chave;
    no->filhos[i + 2] = filhoDir;
    no->qtd++;
}

No *dividir(No *no, int *meio) {
    No *novo = criarNo(no->folha);
    *meio = no->chaves[1];
    novo->chaves[0] = no->chaves[2];
    novo->qtd = 1;
    no->qtd = 1;
    if (!no->folha) {
        novo->filhos[0] = no->filhos[2];
        novo->filhos[1] = no->filhos[3];
        no->filhos[2] = no->filhos[3] = NULL;
    }
    return novo;
}

No *inserirRec(No *no, int chave, int *meio) {
    if (!no) return criarNo(1);
    if (no->folha) {
        inserirEmNo(no, chave, NULL);
    } else {
        int i = no->qtd - 1;
        while (i >= 0 && chave < no->chaves[i]) i--;
        No *filhoNovo = inserirRec(no->filhos[i + 1], chave, meio);
        if (filhoNovo) {
            inserirEmNo(no, *meio, filhoNovo);
            if (no->qtd == MAX) return dividir(no, meio);
        }
    }
    return NULL;
}

No *inserir(No *raiz, int chave) {
    int meio;
    No *novo = inserirRec(raiz, chave, &meio);
    if (!novo) return raiz;
    No *novaRaiz = criarNo(0);
    novaRaiz->chaves[0] = meio;
    novaRaiz->filhos[0] = raiz;
    novaRaiz->filhos[1] = novo;
    novaRaiz->qtd = 1;
    return novaRaiz;
}
