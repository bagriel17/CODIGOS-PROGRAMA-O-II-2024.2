#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAVES 2
#define MAX_FILHOS 3

typedef struct No {
    int chaves[MAX_CHAVES];
    struct No* filhos[MAX_FILHOS];
    int qtd, folha;
} No;

No *criarNo(int folha) {
    No* no = (No*)malloc(sizeof(No));
    no->qtd = 0;
    no->folha = folha;
    for (int i = 0; i <= MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

void percursoEmOrdem(No* no) {
    if (no != NULL) {
        for (int i = 0; i < no->qtd; i++) {
            if (!no->folha) {
                percursoEmOrdem(no->filhos[i]);
            }
            printf("%d ", no->chaves[i]);
        }
        if (!no->folha) {
            percursoEmOrdem(no->filhos[no->qtd]);
        }
    }
}

void inserirEmNo(No* no, int chave, No* filhoDir) {
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

void inserirEmNo(No* no, int chave, No* filhoDir) {
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

No* dividirNo(No* no, int* chavePromovida) {
    No* novoNo = criarNo(no->folha);
    *chavePromovida = no->chaves[1]; // A chave do meio é promovida
    novoNo->chaves[0] = no->chaves[2];
    novoNo->qtd = 1;
    no->qtd = 1;

    if (!no->folha) {
        novoNo->filhos[0] = no->filhos[2];
        novoNo->filhos[1] = no->filhos[3];
        no->filhos[2] = no->filhos[3] = NULL;
    }

    return novoNo;
}

No* inserirRecursivo(No* no, int chave, int* chavePromovida) {
    if (no->folha) {
        inserirEmNo(no, chave, NULL);
    } else {
        int i = no->qtd - 1;
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
        }
        i++;
        No* filhoNovo = inserirRecursivo(no->filhos[i], chave, chavePromovida);
        if (filhoNovo) {
            inserirEmNo(no, *chavePromovida, filhoNovo);
            if (no->qtd == MAX_CHAVES + 1) {
                return dividirNo(no, chavePromovida);
            }
        }
    }
    return NULL;
}

No* inserir(No* raiz, int chave) {
    int chavePromovida;
    No* novoNo = inserirRecursivo(raiz, chave, &chavePromovida);
    if (!novoNo) return raiz;

    No* novaRaiz = criarNo(0);
    novaRaiz->chaves[0] = chavePromovida;
    novaRaiz->filhos[0] = raiz;
    novaRaiz->filhos[1] = novoNo;
    novaRaiz->qtd = 1;
    return novaRaiz;
}

int main() {
    No* raiz = criarNo(1);

    int opcao, valor;
    while (1) {
        printf("\n1. Inserir\n2. Exibir em Ordem\n3. Sair\nEscolha: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Percurso em Ordem: ");
                percursoEmOrdem(raiz);
                printf("\n");
                break;
            case 3:
                exit(0);
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}
