#include <stdio.h>
#include <stdbool.h>

#ifndef BARCODEMANAGER_H
#define BARCODEMANAGER_H

typedef struct {
    char *identificador;
    char *nomeArquivo;
    int espacamento;
    int largura;
    int altura;
} ConfiguracaoBarra;

bool validarIdentificador(char *identificador);         
bool gerar(const ConfiguracaoBarra *configuracao);
bool verificarMarcador(int **matriz, int largura, int altura, int V, char *sequenciaBinaria);
bool verificarCabecalho(FILE *arquivo, int *largura, int *altura);
bool extrairIdentificador(const char *sequenciaBinaria, char *identificador);

#endif

