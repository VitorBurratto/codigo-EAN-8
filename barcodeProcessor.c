#include <ctype.h>
#include <string.h>
#include "barcodeManager.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

 char *padroesLEAN8[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};

 char *padroesREAN8[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

bool gerar (const ConfiguracaoBarra *configuracao) {
    if (!validarIdentificador(configuracao->identificador)) {
    
        return false;
    }
    FILE *arquivo = fopen(configuracao->nomeArquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Falha ao tentar abrir o arquivo %s.\n", configuracao->nomeArquivo);
        return false;
    }

    char *marcadorInicial = "101";
     char *marcadorCentral = "01010";
     char *marcadorFinal = "101";

    int larguraCodigo = configuracao->espacamento * 2 + (3 + 28 + 5 + 28 + 3) * configuracao->largura;
    int alturaCodigo = configuracao->altura + 2 * configuracao->espacamento;

    fprintf(arquivo, "P1\n%d %d\n", larguraCodigo, alturaCodigo);

    // Margem superior
for (int y = 0; y < configuracao->espacamento; y++) {
    for (int x = 0; x < larguraCodigo; x++) {
        fprintf(arquivo, "%s", (x == larguraCodigo - 1) ? "0\n" : "0 ");
    }
}
    for (int y = 0; y < configuracao->altura; y++) {
        
        // Espaçamento lateral
        for (int x = 0; x < configuracao->espacamento; x++) {
            fprintf(arquivo, "0 ");
        }
        for (int i = 0; i < 3; i++) {
             for (int w = 0; w < configuracao->largura; w++) {
                fprintf(arquivo, "%c ", marcadorInicial[i]);
            }
        }
        // Padrões L (4 primeiros dígitos)
        for (int i = 0; i < 4; i++) {
            char *padrao = padroesLEAN8[configuracao->identificador[i] - '0'];
            for (int j = 0; j < 7; j++) {
                for (int w = 0; w < configuracao->largura; w++) {
                    fprintf(arquivo, "%c ", padrao[j]);
                }
            }
        }
        for (int i = 0; i < 5; i++) {
            for (int w = 0; w < configuracao->largura; w++) {
                fprintf(arquivo, "%c ", marcadorCentral[i]);
            }
        }
        // Padrões R (4 últimos dígitos)
        for (int i = 4; i < 8; i++) {
            char *padrao = padroesREAN8[configuracao->identificador[i] - '0'];
            for (int j = 0; j < 7; j++) {
                for (int w = 0; w < configuracao->largura; w++) {
                    fprintf(arquivo, "%c ", padrao[j]);
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int w = 0; w < configuracao->largura; w++) {
                fprintf(arquivo, "%c ", marcadorFinal[i]);
            }
        } 
        // Espaçamento lateral
        for (int x = 0; x < configuracao->espacamento; x++) {
            fprintf(arquivo, "0 ");
        }
        fprintf(arquivo, "\n");
    }
    for (int y = 0; y < configuracao->espacamento; y++) {
        for (int x = 0; x < larguraCodigo; x++) {
            fprintf(arquivo, "0 ");
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
    return true;
}

bool extrairIdentificador(const char *sequenciaBinaria, char *identificador) {
    int posicao = 0;

    for (int i = 3; i < 31; i += 7) {
        bool encontrado = false;
        for (int j = 0; j < 10; j++) {
            if (strncmp(&sequenciaBinaria[i], padroesLEAN8[j], 7) == 0) {
                identificador[posicao++] = '0' + j;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            fprintf(stderr, "Sequencia L-Code invalida.\n");
            return false;
        }
    }
    for (int i = 36; i < 64; i += 7) {
        bool encontrado = false;
        for (int j = 0; j < 10; j++) {
            if (strncmp(&sequenciaBinaria[i], padroesREAN8[j], 7) == 0) {
                identificador[posicao++] = '0' + j;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            fprintf(stderr, "Sequencia R-Code invalida.\n");
            return false;
        }
    }
    identificador[posicao] = '\0';
    return true;
}

bool validarIdentificador(char *identificador) {
    if (strlen(identificador) != 8) {
        fprintf(stderr, "O identificador precisa ter exatamente 8 digitos.\n");
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(identificador[i])) {
            fprintf(stderr, "O identificador contem caracteres nao numerios.\n");
            return false;
        }
    }
    return true;
}

bool verificarCabecalho(FILE *arquivo, int *larguraBarra, int *altura) {
    const char *erros[] = {
        "Nao a nada no arquivo ou foi mal formatado.\n",
        "Falha ao ler o formato.\n",
        "O arquivo nao esta em PBM.\n",
        "Dimensoes invalidas no cabecalho do arquivo.\n"
    };
    char linha[256];
    int caractere;

    do {
        caractere = fgetc(arquivo);
    } while (isspace(caractere));

    if (caractere == EOF) {
        fprintf(stderr, "%s", erros[0]);
        return false;
    }
    ungetc(caractere, arquivo);

    if (!fgets(linha, sizeof(linha), arquivo)) {
        fprintf(stderr, "%s", erros[1]);
        return false;
    }
    linha[strcspn(linha, "\r\n")] = '\0';

    if (strcmp(linha, "P1") != 0) {
        fprintf(stderr, "%s", erros[2]);
        return false;
    }
    if (!fgets(linha, sizeof(linha), arquivo) || sscanf(linha, "%d %d", larguraBarra, altura) != 2) {
        fprintf(stderr, "%s", erros[3]);
        return false;
    }
    printf("%d %d", *larguraBarra, *altura);
    return true;
}

bool verificarMarcador(int **matriz, int larguraCodigo, int altura, int V, char *sequenciaBinaria) {
    int espacamento = (larguraCodigo - (V  * 67)) / 2;
    int posicao = 0;

    // Extrair a sequência binária
    for (int i = espacamento; i < larguraCodigo - espacamento; i += V) {
        int soma = 0;
        for (int j = i; j < i + V; j++) {
            soma += matriz[altura / 2][j];
        }
        sequenciaBinaria[posicao++] = (soma > (V / 2)) ? '1' : '0';
    }
    sequenciaBinaria[posicao] = '\0';

    int marcadorInicial = 0;                    
    int marcadorCentral = 3 + 28;       
    int marcadorFinal = marcadorCentral + 5 + 28; 

const char *erros[] = {
    "Marcador de inicio errado.\n",
    "Marcador central errado.\n",
    "Marcador de fim errado.\n"
};
if (strncmp(&sequenciaBinaria[marcadorInicial], "101", 3) != 0) {
    printf("%s", erros[0]);
    return false;
}
if (strncmp(&sequenciaBinaria[marcadorCentral], "01010", 5) != 0) {
    printf("%s", erros[1]);
    return false;
}
if (strncmp(&sequenciaBinaria[marcadorFinal], "101", 3) != 0) {
    printf("%s", erros[2]);
    return false;
}
    return true;
}

