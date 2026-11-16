#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jogo.h"

// Cores do terminal
#define VERMELHO "\033[31m"
#define VERDE    "\033[32m"
#define AMARELO  "\033[33m"
#define AZUL     "\033[36m"
#define RESET    "\033[0m"

// Retira a quebra de linha
void limparLinha(char texto[]) {
    texto[strcspn(texto, "\r\n")] = '\0';
}

// Deixa a resposta em minusculo e retira espacos e hifens
void normalizarResposta(char texto[]) {
    int i, j = 0;

    for (i = 0; texto[i] != '\0'; i++) {
        unsigned char caractere = (unsigned char) texto[i];

        if (!isspace(caractere) && caractere != '-') {
            texto[j] = (char) tolower(caractere);
            j++;
        }
    }

    texto[j] = '\0';
}

// Carrega os poemas do arquivo TXT
int carregarPoemas(Pilar pilares[]) {
    FILE *arquivo;
    char linha[500];
    int i;

    arquivo = fopen("dados/poemas.txt", "r");

    if (arquivo == NULL) {
        printf(VERMELHO "\nErro ao abrir poemas.txt!\n" RESET);
        return 0;
    }

    for (i = 0; i < TOTAL_PILARES; i++) {

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            printf(VERMELHO "\nFaltam poemas no arquivo!\n" RESET);
            fclose(arquivo);
            return 0;
        }

        limparLinha(linha);

        strcpy(pilares[i].poema, linha);
    }

    fclose(arquivo);

    return 1;
}

// Mostra a representacao dos quatro pilares usando matriz
void mostrarSala(void) {
    int mapa[2][2] = {
        {1, 2},
        {3, 4}
    };

    int i, j;

    printf("\n");
    printf(AMARELO "       MAPA DA SALA\n" RESET);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("       [ PILAR %d ]", mapa[i][j]);
        }

        printf("\n");
    }

    printf("\n             [ PORTA ]\n");
}

// Salva a pontuacao no arquivo
void salvarRanking(Jogador jogador, const char resultado[]) {
    FILE *arquivo;

    arquivo = fopen("dados/ranking.txt", "a");

    if (arquivo == NULL) {
        printf(VERMELHO "\nErro ao salvar ranking!\n" RESET);
        return;
    }

    fprintf(arquivo, "%s;%d;%s\n",
            jogador.nome,
            jogador.pontos,
            resultado);

    fclose(arquivo);
}

// Inicia a primeira fase
void iniciarJogo(void) {

    Jogador jogador = {0};

    Pilar pilares[TOTAL_PILARES] = {
        {"Caipora", "", "caipora", 0},
        {"Cumade Fulozinha", "", "cumadefulozinha", 0},
        {"Papa-Figo", "", "papafigo", 0},
        {"Cabra Cabriola", "", "cabracabriola", 0}
    };

    char resposta[100];

    int i;

    jogador.vida = 100;
    jogador.pontos = 0;
    jogador.pilaresResolvidos = 0;

    strcpy(jogador.personagem.apelido, "Viajante");

    jogador.personagem.energia = 100;
    jogador.personagem.nivel = 1;

    // Solicita o nome do jogador
    printf("\nDigite seu nome: ");

    fgets(jogador.nome, sizeof(jogador.nome), stdin);

    limparLinha(jogador.nome);

    if (strlen(jogador.nome) == 0) {
        strcpy(jogador.nome, "Viajante");
    }

    // Carrega os poemas
    if (!carregarPoemas(pilares)) {
        return;
    }

    printf("\n" AZUL "====================================\n");
    printf("       A SALA DOS QUATRO PILARES\n");
    printf("====================================\n" RESET);

    printf("\nVoce acorda em uma sala misteriosa...\n");
    printf("Quatro pilares estao diante de voce.\n");
    printf("Uma porta permanece fechada.\n");

    printf("\n%s, sua vida comeca em 100 pontos.\n",
           jogador.nome);

    mostrarSala();

    // Percorre os quatro pilares
    for (i = 0; i < TOTAL_PILARES && jogador.vida > 0; i++) {

        printf("\n" AMARELO "====================================\n");
        printf("              PILAR %d\n", i + 1);
        printf("====================================\n" RESET);

        printf("\nUm poema esta gravado na pedra:\n\n");

        printf("\"%s\"\n", pilares[i].poema);

        // Permite novas tentativas enquanto estiver vivo
        while (pilares[i].resolvido == 0 && jogador.vida > 0) {

            printf("\nQual criatura esta sendo descrita?\n");
            printf("Sua resposta: ");

            if (fgets(resposta, sizeof(resposta), stdin) == NULL) {
                jogador.vida = 0;
                break;
            }

            limparLinha(resposta);
            normalizarResposta(resposta);

            if (strcmp(resposta, pilares[i].resposta) == 0) {

                printf(VERDE "\nRESPOSTA CORRETA!\n" RESET);

                printf("O pilar comeca a brilhar...\n");

                pilares[i].resolvido = 1;

                jogador.pontos += 100;
                jogador.pilaresResolvidos++;

                jogador.personagem.nivel++;

            } else {

                jogador.vida -= 25;

                if (jogador.vida < 0) {
                    jogador.vida = 0;
                }

                jogador.personagem.energia = jogador.vida;

                printf(VERMELHO "\nRESPOSTA ERRADA!\n" RESET);

                printf("Voce recebeu 25 pontos de dano.\n");

                printf("Vida restante: %d\n", jogador.vida);
            }
        }
    }

    // Resultado da partida
    printf("\n" AZUL "====================================\n");
    printf("           FIM DA FASE\n");
    printf("====================================\n" RESET);

    if (jogador.vida > 0 &&
        jogador.pilaresResolvidos == TOTAL_PILARES) {

        printf(VERDE "\nA PORTA SE ABRIU!\n" RESET);

        printf("Voce conseguiu escapar da sala!\n");

        printf("Parabens, %s!\n", jogador.nome);

        salvarRanking(jogador, "VITORIA");

    } else {

        printf(VERMELHO "\nVOCE MORREU!\n" RESET);

        printf("A sala consumiu sua ultima energia.\n");

        salvarRanking(jogador, "DERROTA");
    }

    printf("\nPontuacao: %d pontos\n", jogador.pontos);
    printf("Pilares resolvidos: %d de 4\n",
           jogador.pilaresResolvidos);

    printf("Vida final: %d\n", jogador.vida);

}

// Exibe o ranking
void exibirRanking(void) {

    FILE *arquivo;
    char linha[150];

    arquivo = fopen("dados/ranking.txt", "r");

    printf("\n" AMARELO "======= RANKING DE PARTIDAS =======\n" RESET);

    if (arquivo == NULL) {
        printf("Ainda nao existem partidas registradas.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }

    fclose(arquivo);
}

// Exibe as instrucoes
void exibirInstrucoes(void) {

    printf("\n" AZUL "======= COMO JOGAR =======\n" RESET);

    printf("\n1. Voce comeca com 100 pontos de vida.\n");
    printf("2. Existem quatro pilares na sala.\n");
    printf("3. Cada pilar possui um poema.\n");
    printf("4. Cada resposta correta vale 100 pontos.\n");
    printf("5. Cada resposta errada retira 25 pontos de vida.\n");
    printf("6. Voce pode tentar novamente enquanto estiver vivo.\n");
    printf("7. Acerte os quatro pilares para abrir a porta.\n");
    printf("8. Se sua vida chegar a zero, voce perde.\n");
}