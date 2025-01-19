#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "barcodeManager.h"

#define NOMEARQUIVO_DEFAULT "codigoEAN8.pbm"
#define ESPACAMENTO_DEFAULT 4
#define LARGURA_DEFAULT 3
#define ALTURA_DEFAULT 50

// Função para obter valor ou valor padrão (caso seja um traço '-')
int obterValorPadrao(const char *arg, int valorPadrao) {
    return (strcmp(arg, "-") == 0) ? valorPadrao : atoi(arg);
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <Identificador EAN-8> [Arquivo.pbm] [Espaçamento (Opcional)] [Largura (Opcional)] [Altura (Opcional)]\n", argv[0]);
        return EXIT_FAILURE;
    }
    ConfiguracaoBarra configuracao = {
        .identificador = argv[1], 
        .nomeArquivo = (argc > 2 && strcmp(argv[2], "-") != 0) ? argv[2] : NOMEARQUIVO_DEFAULT, 
        .espacamento = (argc > 3) ? obterValorPadrao(argv[3], ESPACAMENTO_DEFAULT) : ESPACAMENTO_DEFAULT,
        .largura = (argc > 4) ? obterValorPadrao(argv[4], LARGURA_DEFAULT) : LARGURA_DEFAULT,
        .altura = (argc > 5) ? obterValorPadrao(argv[5], ALTURA_DEFAULT) : ALTURA_DEFAULT    
    };
    const char *extensao = strrchr(configuracao.nomeArquivo, '.');
    if (!extensao || strcmp(extensao, ".pbm") != 0) {
        fprintf(stderr, "O arquivo nao tem extensao .pbm\n");
        return EXIT_FAILURE;
    }
    if (configuracao.espacamento <= 0 || configuracao.largura <= 0 || configuracao.altura <= 0) {
        fprintf(stderr, "Todos os valores precisam ser positivos.\n");
        return EXIT_FAILURE;
    }
    if (configuracao.largura > 30 || configuracao.altura > 500) {
        fprintf(stderr, "Largura e altura precisam ser validos (largura <= 30, altura <= 500).\n");
        return 1;
    }
    if (strlen(configuracao.identificador) != 8 || strspn(configuracao.identificador, "0123456789") != 8) {
        fprintf(stderr, "O identificador precisa ter 8 digitos.\n");
        return EXIT_FAILURE;
    }
    FILE *arquivo = fopen(configuracao.nomeArquivo, "r");
    if (arquivo) {
        fclose(arquivo);
        printf("O arquivo '%s' ja existe. Tem certeza? (s/n): ", configuracao.nomeArquivo);

        char resposta;
        if (scanf(" %c", &resposta) != 1) {
            fprintf(stderr, "Responda com (s/n).\n");
            return EXIT_FAILURE;
        }

        if (resposta != 's' && resposta != 'S') {
            printf("Operaçao cancelada.\n");
            return EXIT_FAILURE;
        }
    }
    if (!gerar(&configuracao)) {
        return EXIT_FAILURE;
    }
    printf("Imagem do codigo EAN8 foi gerada. Agora eh possivel acessar o arquivo: %s\n", configuracao.nomeArquivo);
    return EXIT_SUCCESS;
}