
#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

int main(void) {

    int opcao;
    char entrada[20];

    do {

        printf("\n");
        printf("\033[33m");
        printf("====================================\n");
        printf("     A GRANDE JORNADA PELO NORDESTE\n");
        printf("====================================\n");
        printf("\033[0m");

        printf("\n1 - Iniciar nova jornada\n");
        printf("2 - Consultar ranking\n");
        printf("3 - Como jogar\n");
        printf("4 - Sair\n");

        printf("\nEscolha uma opcao: ");

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }

        opcao = atoi(entrada);

        switch (opcao) {

            case 1:
                iniciarJogo();
                break;

            case 2:
                exibirRanking();
                break;

            case 3:
                exibirInstrucoes();
                break;

            case 4:
                printf("\nAte a proxima aventura!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 4);

    return 0;
}
