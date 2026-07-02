#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Cria dinamicamente uma sala com nome e pista opcional
Sala* criarSala(char nome[], char pista[]) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// Insere uma pista na BST em ordem alfabética
PistaNode* inserirPista(PistaNode *raiz, char pista[]) {
    if (pista[0] == '\0') {
        return raiz; 
    }

    if (raiz == NULL) {
        PistaNode *novoNo = (PistaNode*) malloc(sizeof(PistaNode));

        if (novoNo == NULL) {
            printf("Erro ao alocar memoria.\n");
            exit(1);
        }

        strcpy(novoNo->pista, pista);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;

        return novoNo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// Exibe as pistas em ordem alfabética usando percurso em ordem
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) {
        return;
    }

    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Controla a exploração da mansão e coleta as pistas encontradas
void explorarSalasComPistas(Sala *salaAtual, PistaNode **arvorePistas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVoce esta em: %s\n", salaAtual->nome);

        if (salaAtual->pista[0] != '\0') {
            printf("Pista encontrada: %s\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        } else {
            printf("Nenhuma pista encontrada neste comodo.\n");
        }

        printf("\nCaminhos disponiveis:\n");

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
            printf("\nExploracao encerrada.\n");
            return;
        } else {
            printf("\nOpcao invalida.\n");
        }
    }
}

// Libera a árvore da mansão
void liberarMansao(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);

    free(raiz);
}

// Libera a árvore de pistas
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);

    free(raiz);
}

int main() {
    PistaNode *arvorePistas = NULL;

    // Mapa fixo da mansão
    Sala *hall = criarSala("Hall de Entrada", "Pegadas molhadas no tapete");

    hall->esquerda = criarSala("Sala de Estar", "Bilhete rasgado no sofa");
    hall->direita = criarSala("Corredor Principal", "");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora da estante");
    hall->esquerda->direita = criarSala("Cozinha", "Faca limpa recentemente");

    hall->direita->esquerda = criarSala("Escritorio", "Cofre aberto");
    hall->direita->direita = criarSala("Jardim", "Pegadas perto da janela");

    printf("========== DETECTIVE QUEST ==========\n");
    printf("Explore a mansao e colete pistas pelo caminho.\n");

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n========== PISTAS COLETADAS EM ORDEM ALFABETICA ==========\n");

    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    liberarMansao(hall);
    liberarPistas(arvorePistas);

    return 0;
}