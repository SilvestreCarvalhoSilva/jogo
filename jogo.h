#ifndef JOGO_H
#define JOGO_H

#define TOTAL_PILARES 4

// Dados do personagem
typedef struct {
    char apelido[30];
    int energia;
    int nivel;
} Personagem;

// Dados do jogador
typedef struct {
    char nome[50];
    int vida;
    int pontos;
    int pilaresResolvidos;

    // Struct aninhada
    Personagem personagem;

} Jogador;

// Dados dos pilares
typedef struct {
    char criatura[40];
    char poema[500];
    char resposta[40];
    int resolvido;
} Pilar;

// Funcoes do jogo
void iniciarJogo(void);
void exibirRanking(void);
void exibirInstrucoes(void);

#endif