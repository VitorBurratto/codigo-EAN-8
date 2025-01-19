#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "barcodeManager.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "O nome do arquivo deve ser informado.\n");
        return 1;
    }

    const char *nomeArquivo = argv[1];
    FILE *arquivo = fopen(nomeArquivo, "r");

    int largura, altura;

    if (!verificarCabecalho(arquivo, &largura, &altura)) {
        fclose(arquivo);
        return 1;
    }

    int **matriz = (int **)malloc(altura * sizeof(int *));
    for (int i = 0; i < altura; i++) {
        matriz[i] = (int *)malloc(largura * sizeof(int));
    }

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (fscanf(arquivo, "%d", &matriz[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler o arquivo PBM.\n");
                fclose(arquivo);
                for (int k = 0; k <= i; k++) free(matriz[k]);
                free(matriz);
                return 1;
            }
        }
    }
    fclose(arquivo);

    int V = largura / 67;

    char sequenciaBinaria[68];
    if (verificarMarcador(matriz, largura, altura, V, sequenciaBinaria)) {

        char identificador[9];
        if (extrairIdentificador(sequenciaBinaria, identificador)) {
            printf("\nIdentificador extraido: %s\n", identificador);
        } else {
            printf("Erro ao extrair os digitos.\n");
        }
        printf("Sequencia binaria: %s\n", sequenciaBinaria);

    } else {
        printf("Codigo de barras invalido.\n");
    }
    for (int i = 0; i < altura; i++) free(matriz[i]);
    free(matriz);
    
    return 0;
}