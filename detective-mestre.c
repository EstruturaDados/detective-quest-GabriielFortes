#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

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

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

HashNode *tabelaHash[TAM_HASH];

// Cria dinamicamente um cômodo da mansão
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

// Calcula o índice da tabela hash com base na pista
int funcaoHash(char pista[]) {
    int soma = 0;

    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }

    return soma % TAM_HASH;
}

// Insere a associação pista -> suspeito na tabela hash
void inserirNaHash(char pista[], char suspeito[]) {
    int indice = funcaoHash(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// Consulta na hash qual suspeito está ligado a uma pista
char* encontrarSuspeito(char pista[]) {
    int indice = funcaoHash(pista);

    HashNode *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }

        atual = atual->proximo;
    }

    return NULL;
}

// Insere uma pista coletada na BST
PistaNode* inserirPista(PistaNode *raiz, char pista[]) {
    if (pista[0] == '\0') {
        return raiz;
    }

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));

        if (novo == NULL) {
            printf("Erro ao alocar memoria.\n");
            exit(1);
        }

        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;

        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// Exibe pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) {
        return;
    }

    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Navega pela mansão e coleta pistas
void explorarSalas(Sala *salaAtual, PistaNode **arvorePistas) {
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

        printf("s - Encerrar exploracao\n");
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

// Conta quantas pistas coletadas apontam para o suspeito acusado
int contarPistasDoSuspeito(PistaNode *raiz, char suspeitoAcusado[]) {
    if (raiz == NULL) {
        return 0;
    }

    int total = 0;

    char *suspeitoEncontrado = encontrarSuspeito(raiz->pista);

    if (suspeitoEncontrado != NULL && strcmp(suspeitoEncontrado, suspeitoAcusado) == 0) {
        total++;
    }

    total += contarPistasDoSuspeito(raiz->esquerda, suspeitoAcusado);
    total += contarPistasDoSuspeito(raiz->direita, suspeitoAcusado);

    return total;
}

// Fase final: jogador acusa um suspeito e o sistema avalia as pistas
void verificarSuspeitoFinal(PistaNode *arvorePistas) {
    char suspeitoAcusado[50];

    printf("\n========== JULGAMENTO FINAL ==========\n");

    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada. Nao e possivel acusar ninguem.\n");
        return;
    }

    printf("Pistas coletadas:\n");
    exibirPistas(arvorePistas);

    printf("\nSuspeitos possiveis: Mordomo, Jardineira, Cozinheiro\n");
    printf("Digite o nome do suspeito acusado: ");

    scanf(" %[^\n]", suspeitoAcusado);

    int quantidadePistas = contarPistasDoSuspeito(arvorePistas, suspeitoAcusado);

    printf("\nPistas que apontam para %s: %d\n", suspeitoAcusado, quantidadePistas);

    if (quantidadePistas >= 2) {
        printf("Acusacao sustentada! Existem pistas suficientes contra o suspeito.\n");
    } else {
        printf("Acusacao fraca! Nao ha pistas suficientes contra esse suspeito.\n");
    }
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

void liberarMansao(Sala *raiz) {
    if (raiz == NULL) return;

    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);

    free(raiz);
}

void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);

    free(raiz);
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *atual = tabelaHash[i];

        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

int main() {
    PistaNode *arvorePistas = NULL;

    inicializarHash();

    inserirNaHash("Pegadas molhadas no tapete", "Mordomo");
    inserirNaHash("Chave escondida no vaso", "Mordomo");
    inserirNaHash("Luvas sujas de terra", "Jardineira");
    inserirNaHash("Pegadas perto da janela", "Jardineira");
    inserirNaHash("Faca limpa recentemente", "Cozinheiro");
    inserirNaHash("Avental com mancha escura", "Cozinheiro");

    Sala *hall = criarSala("Hall de Entrada", "Pegadas molhadas no tapete");

    hall->esquerda = criarSala("Sala de Estar", "Chave escondida no vaso");
    hall->direita = criarSala("Corredor Principal", "");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Luvas sujas de terra");
    hall->esquerda->direita = criarSala("Cozinha", "Faca limpa recentemente");

    hall->direita->esquerda = criarSala("Escritorio", "Avental com mancha escura");
    hall->direita->direita = criarSala("Jardim", "Pegadas perto da janela");

    printf("========== DETECTIVE QUEST - NIVEL MESTRE ==========\n");
    printf("Explore a mansao, colete pistas e descubra o culpado.\n");

    explorarSalas(hall, &arvorePistas);

    verificarSuspeitoFinal(arvorePistas);

    liberarMansao(hall);
    liberarPistas(arvorePistas);
    liberarHash();

    return 0;
}