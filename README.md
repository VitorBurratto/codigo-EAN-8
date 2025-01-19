<h1 align="center"> 📊 Gerador de Códigos de Barras EAN-8 🌐 </h1>

Este projeto é um gerador de códigos de barras baseado no padrão EAN-8, que salva os códigos no formato PBM (Portable Bitmap) e extrai.

## 🔧 Funcionalidades do Projeto

- **Identificador Obrigatório**: O usuário deve informar um código de 8 dígitos como entrada.

- **Parâmetros Opcionais**:
  - Nome do arquivo de saída (PBM).
  - Espaçamento ao redor do código (em pixels).
  - Largura de cada barra (em pixels).
  - Altura do código de barras (em pixels).

- **Valores Padrão**: Caso algum parâmetro não seja fornecido, o programa utiliza os seguintes valores:
  - Nome do Arquivo: `codigoEAN8.pbm`
  - Espaçamento: `4`
  - Largura: `3`
  - Altura: `50`

- **Validação de Identificador**: Verifica se o código fornecido tem 8 dígitos numéricos e valida o dígito verificador.

- **Aviso de Sobrescrita**: Caso o arquivo já exista, o programa pede confirmação antes de sobrescrevê-lo.


## 📁 Estrutura do Projeto

A estrutura básica do projeto está organizada da seguinte forma:

>     ├── codigoEAN8.pbm        # Código padrão gerado
>     ├── barcodeManger.h       # Declaração das funções compartilhadas.
>     ├── barcodeProcessor.c    # Implementação das funções de validação e geração.
>     ├── extrair.c             # Programa que extrai o código de barras.
>     ├── extrair.exe    
>     ├── gerar.c               # Programa que gera a imagem do código de barras.
>     ├── gerar.exe     
>     └── README.md

## 🛠 Como Rodar o Projeto

### ⚙️ Compilação
Para compilar o programa, execute esse dois comando em seu terminal para compilar cada program separadamente:

```bash
gcc -o gerar gerar.c barcodeProcessor.c    
```

```bash
gcc -o extrair extrair.c barcodeProcessor.c 
```

### 🚀 Execução 
Para executar o programa:
```bash
./gerar < identificador > [nomeArquivo] [espaçamento] [largura] [altura]
```
- `<identificador>`: Código EAN-8 (obrigatório).
- `[nomeArquivo]`: Nome do arquivo de saída (opcional).
- `[espaçamento]`: Espaçamento ao redor (opcional).
- `[largura]`: Largura das barras (opcional).
- `[altura]`: Altura do código (opcional).


## ❓ Como Utilizar o Opcional

Se você deseja usar apenas alguns parâmetros e deixar os outros como padrão, basta:

- Fornecer `-` para os parâmetros que deseja ignorar, sempre respectivamente.

- Exemplo:
  ```bash
  ./gerar 40170725 - - - 60
  ```
  Neste caso:
  - Nome do Arquivo: `codigoEAN8.pbm` (padrão).
  - Espaçamento: `4` (padrão).
  - Largura: `3` (padrão).
  - Altura: `60`.
  

### 📝 Exemplos Gerar Codigo EAN-8
#### Exemplo 1: Somente Identificador
```bash
./gerar 40170725
```
Usa os valores padrão para nome do arquivo, espaçamento, largura e altura.

#### Exemplo 2: Com Altura Personalizada
```bash
./gerar 12345678 - - 4 -
```
Define apenas a largura como `4`, mantendo os outros valores padrão.

#### Exemplo 3: Todos os Parâmetros
```bash
./gerar 99887766 meuCodigo.pbm 8 5 140
```
Gera um código de barras personalizado com todos os valores definidos.

### 📠 Exemplo Para Extrair o codigo depois de já ter criado o arquivo pbm

```bash
./extrair codigoEAN8.pbm
```
Tem que ser um arquivo válido já gerado pelo comando ./gerar < identificador >


## 🧑‍💻Autor 

<img  src="https://github.com/VitorBurratto.png"  alt="Vítor Burratto"  width="100"  height="100"/> **[Vítor Burratto](https://github.com/VitorBurratto)**
