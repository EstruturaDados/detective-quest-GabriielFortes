#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria uma nova sala dinamicamente usando malloc
Sala* criarSala(char nome[]) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// Permite que o jogador explore a árvore a partir do Hall
void explorarSalas(Sala *salaAtual) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVoce esta em: %s\n", salaAtual->nome);

        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este comodo nao possui mais caminhos. Fim da exploracao.\n");
            return;
        }

        printf("\nEscolha um caminho:\n");

        if (salaAtual->esquerda != NULL) {
            printf("e - Ir para a esquerda: %s\n", salaAtual->esquerda->nome);
        }

        if (salaAtual->direita != NULL) {
            printf("d - Ir para a direita: %s\n", salaAtual->direita->nome);
        }

        printf("s - Sair da exploracao\n");
        printf("Opcao: ");

        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("\nNao existe caminho a esquerda.\n");
            }
        } else if (opcao == 'd') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("\nNao existe caminho a direita.\n");
            }
        } else if (opcao == 's') {
            printf("\nExploracao encerrada pelo jogador.\n");
            return;
        } else {
            printf("\nOpcao invalida.\n");
        }
    }
}

// Libera a memória usada pela árvore
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);

    free(raiz);
}

int main() {
    // Montagem manual do mapa da mansão
    Sala *hall = criarSala("Hall de entrada");

    hall->esquerda = criarSala("Sala de estar");
    hall->direita = criarSala("Corredor principal");

    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Cozinha");

    hall->direita->esquerda = criarSala("Escritorio");
    hall->direita->direita = criarSala("Jardim");

    hall->esquerda->esquerda->esquerda = criarSala("Quarto secreto");
    hall->direita->direita->direita = criarSala("Garagem");

    printf("========== DETECTIVE QUEST ==========\n");
    printf("Explore a mansao e encontre pistas pelo caminho.\n");

    explorarSalas(hall);

    liberarArvore(hall);

    return 0;
}