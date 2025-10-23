#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais ---
#define TAMANHO_FILA 5 // Tamanho fixo da fila de peças futuras

// --- Estrutura de Dados ---

// Estrutura para representar uma peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca pecas[TAMANHO_FILA]; // Array para armazenar as peças
    int frente;               // Índice da frente da fila (onde a próxima peça a ser jogada está)
    int tras;                 // Índice da traseira da fila (onde a próxima peça será inserida)
    int contador;             // Número atual de elementos na fila
    int proximo_id;           // Próximo ID a ser atribuído a uma nova peça
} FilaPecas;

// --- Protótipos das Funções ---
void inicializarFila(FilaPecas *fila);
Peca gerarPeca(int *proximo_id);
int estaCheia(FilaPecas *fila);
int estaVazia(FilaPecas *fila);
void enfileirar(FilaPecas *fila, Peca novaPeca);
Peca desenfileirar(FilaPecas *fila);
void exibirFila(FilaPecas *fila);
void exibirMenu(void);

// --- Implementação das Funções ---

/**
 * Inicializa a estrutura da fila, preparando-a para uso.
 * @param fila Ponteiro para a estrutura FilaPecas.
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->contador = 0;
    fila->proximo_id = 0;
    // Preenche a fila inicialmente com o número fixo de elementos
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca nova = gerarPeca(&(fila->proximo_id));
        // A lógica de inicialização precisa ser cuidadosa para preencher a fila
        // sem usar diretamente o enfileirar, que depende do estado já inicializado.
        // No entanto, para simplicidade, vamos usar o enfileirar se ele for robusto,
        // mas é mais claro preencher o array e ajustar o contador.
        fila->pecas[i] = nova;
        fila->contador++;
        // Como preenchemos sequencialmente, 'tras' aponta para o próximo local vago (o primeiro)
        // e 'frente' aponta para o primeiro elemento (o primeiro).
    }
    fila->frente = 0;
    // O 'tras' aponta para o local *após* o último elemento.
    // Como a fila está cheia (5 elementos), o 'tras' deve ser 5 % 5 = 0.
    fila->tras = fila->contador % TAMANHO_FILA; 
}

/**
 * Gera uma nova peça com um tipo aleatório e um ID sequencial.
 * @param proximo_id Ponteiro para o contador de ID global.
 * @return A nova Peca gerada.
 */
Peca gerarPeca(int *proximo_id) {
    // Semente para o gerador de números pseudo-aleatórios (melhor ser chamada uma vez no main)
    // srand(time(NULL)); 
    
    // Tipos de peças: 'I', 'O', 'T', 'L'
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4; // Escolhe um tipo aleatoriamente
    
    Peca novaPeca;
    novaPeca.nome = tipos[indice];
    novaPeca.id = (*proximo_id);
    (*proximo_id)++; // Incrementa o ID para a próxima peça
    
    return novaPeca;
}

/**
 * Verifica se a fila de peças está cheia.
 * @param fila Ponteiro para a estrutura FilaPecas.
 * @return 1 se cheia, 0 caso contrário.
 */
int estaCheia(FilaPecas *fila) {
    return fila->contador == TAMANHO_FILA;
}

/**
 * Verifica se a fila de peças está vazia.
 * @param fila Ponteiro para a estrutura FilaPecas.
 * @return 1 se vazia, 0 caso contrário.
 */
int estaVazia(FilaPecas *fila) {
    return fila->contador == 0;
}

/**
 * Adiciona uma nova peça ao final da fila (enqueue).
 * @param fila Ponteiro para a estrutura FilaPecas.
 * @param novaPeca A peça a ser inserida.
 */
void enfileirar(FilaPecas *fila, Peca novaPeca) {
    if (estaCheia(fila)) {
        printf("\nERRO: Fila de peças cheia! Não é possível inserir nova peça.\n");
        return;
    }
    
    fila->pecas[fila->tras] = novaPeca;
    // O operador módulo garante que 'tras' volte ao início (0) quando atingir o limite do array
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->contador++;
    printf("\nSUCESSO: Peça [%c %d] inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
}

/**
 * Remove e retorna a peça da frente da fila (dequeue).
 * @param fila Ponteiro para a estrutura FilaPecas.
 * @return A Peca removida. Se a fila estiver vazia, retorna uma peça inválida.
 */
Peca desenfileirar(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', -1}; // Peça inválida para erro
    
    if (estaVazia(fila)) {
        printf("\nERRO: Fila de peças vazia! Não há peça para jogar.\n");
        return pecaRemovida;
    }
    
    pecaRemovida = fila->pecas[fila->frente];
    // O operador módulo garante que 'frente' volte ao início (0) quando atingir o limite
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->contador--;
    
    printf("\nSUCESSO: Peça jogada (removida da frente): [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
    return pecaRemovida;
}

/**
 * Exibe o estado atual da fila, mostrando todas as peças.
 * @param fila Ponteiro para a estrutura FilaPecas.
 */
void exibirFila(FilaPecas *fila) {
    printf("\n--- Confiram seu estado: ---\n");
    if (estaVazia(fila)) {
        printf("Fila de peças está vazia!\n");
        return;
    }
    
    printf("Fila de peças: ");
    int i = fila->frente;
    int elementos = fila->contador;
    
    while (elementos > 0) {
        printf("[%c %d]", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % TAMANHO_FILA; // Avança para o próximo índice (circular)
        elementos--;
        if (elementos > 0) {
            printf(" ");
        }
    }
    printf("\n------------------------------\n");
}

/**
 * Exibe o menu de opções para o jogador.
 */
void exibirMenu(void) {
    printf("\nOpções de ação:\n");
    printf("Código | Ação\n");
    printf("-------|---------------------------\n");
    printf(" 1     | Jogar peça (dequeue)\n");
    printf(" 2     | Inserir nova peça (enqueue)\n");
    printf(" 0     | Sair\n");
    printf("Escolha sua opção: ");
}

// --- Função Principal ---

int main() {
    // Inicializa a semente do gerador de números aleatórios APENAS UMA VEZ
    srand(time(NULL)); 
    
    FilaPecas fila;
    
    // 1. Inicializa a fila de peças com 5 elementos
    inicializarFila(&fila);
    
    int opcao = -1;
    
    printf("--- Tetris Stack: Simulador da Fila de Peças Futuras ---\n");
    
    while (opcao != 0) {
        exibirFila(&fila); // Exibe o estado da fila antes de cada ação
        exibirMenu();
        
        // Leitura da opção do usuário
        if (scanf("%d", &opcao) != 1) {
            // Tratamento de erro para entrada não numérica
            printf("\nOpção inválida. Por favor, digite um número.\n");
            // Limpa o buffer de entrada para evitar loop infinito
            while (getchar() != '\n');
            opcao = -1; // Garante que o loop continue
            continue;
        }
        
        switch (opcao) {
            case 1: // Jogar peça (dequeue)
                desenfileirar(&fila);
                break;
                
            case 2: { // Inserir nova peça (enqueue)
                // A peça é gerada automaticamente
                Peca nova = gerarPeca(&(fila.proximo_id));
                enfileirar(&fila, nova);
                break;
            }
                
            case 0: // Sair
                printf("\nSaindo do simulador. Obrigado por jogar Tetris Stack!\n");
                break;
                
            default:
                printf("\nOpção inválida. Por favor, escolha 0, 1 ou 2.\n");
                break;
        }
    }
    
    return 0;
}