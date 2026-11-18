#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define TAM 10
#define MAX 10000

typedef struct {
    int comparacoes;
    int movimentos;
    double segundos;
} Resultado;

void exibirCabecalho(void);
void lerPontuacoes(int v[], int n);
void copiarVetor(int origem[], int destino[], int n);
void exibirVetor(int v[], int n);
int bubbleSort(int v[], int n);
int selectionSort(int v[], int n);
int insertionSort(int v[], int n);
int vetoresIguais(int a[], int b[], int n);

int lerInteiro(const char mensagem[], int minimo, int maximo);
void gerarPontuacoes(int v[], int n);
void exibirAmostra(int v[], int n);
int foraDeOrdem(int a, int b, int ordem);
Resultado bubbleDetalhado(int v[], int n, int ordem);
Resultado selectionDetalhado(int v[], int n, int ordem);
Resultado insertionDetalhado(int v[], int n, int ordem);
Resultado ordenar(int v[], int n, int metodo, int ordem);

int main(void)
{
    int pontuacoes[MAX], bubble[MAX], selection[MAX], insertion[MAX];

    int *vetores[3] = {bubble, selection, insertion};

    const char *nomes[3] = {
        "Bubble Sort",
        "Selection Sort",
        "Insertion Sort"
    };

    Resultado resultados[3];
    int opcao, n, ordem, metodo, i, escolhido;
    double mediana;

    srand((unsigned int)time(NULL));
    exibirCabecalho();

    while (1) {
        printf("\n1 - Digitar dez pontuacoes\n");
        printf("2 - Gerar dez pontuacoes aleatorias\n");
        printf("3 - Testar tempo com um vetor maior e aleatorio\n");
        printf("0 - Sair\n");

        opcao = lerInteiro("Opcao: ", 0, 3);

        if (opcao == 0) {
            break;
        }

        n = TAM;

        if (opcao == 3) {
            n = lerInteiro(
                "Quantidade de valores (100 a 10000): ",
                100,
                MAX
            );
        }

        if (opcao == 1) {
            lerPontuacoes(pontuacoes, n);
        } else {
            gerarPontuacoes(pontuacoes, n);
        }

        printf("\n1 - Ordem crescente\n");
        printf("2 - Ordem decrescente\n");
        ordem = lerInteiro("Ordem: ", 1, 2);

        printf("\n1 - Bubble Sort\n");
        printf("2 - Selection Sort\n");
        printf("3 - Insertion Sort\n");
        printf("4 - Comparar os tres\n");
        metodo = lerInteiro("Metodo: ", 1, 4);

        printf("\nOriginal: ");
        exibirAmostra(pontuacoes, n);

        copiarVetor(pontuacoes, bubble, n);
        copiarVetor(pontuacoes, selection, n);
        copiarVetor(pontuacoes, insertion, n);

        for (i = 0; i < 3; i++) {
            if (metodo == 4 || metodo == i + 1) {
                resultados[i] = ordenar(
                    vetores[i], n, i + 1, ordem
                );

                printf("\n%s: ", nomes[i]);
                exibirAmostra(vetores[i], n);

                printf(
                    "Comparacoes: %d | %s: %d | Tempo de CPU: %.6f s\n",
                    resultados[i].comparacoes,
                    i == 2 ? "Deslocamentos" : "Trocas",
                    resultados[i].movimentos,
                    resultados[i].segundos
                );
            }
        }

        if (metodo == 4) {
            printf(
                "\nResultados equivalentes: %s\n",
                vetoresIguais(bubble, selection, n) &&
                vetoresIguais(bubble, insertion, n)
                    ? "SIM" : "NAO"
            );
        }

        escolhido = metodo == 4 ? 0 : metodo - 1;

        mediana = (
            vetores[escolhido][(n - 1) / 2] +
            vetores[escolhido][n / 2]
        ) / 2.0;

        printf(
            "\nMenor valor do vetor: %d\n",
            vetores[escolhido][ordem == 1 ? 0 : n - 1]
        );

        printf(
            "Maior valor do vetor: %d\n",
            vetores[escolhido][ordem == 1 ? n - 1 : 0]
        );

        printf("Mediana: %.2f\n", mediana);
    }

    return 0;
}

void exibirCabecalho(void)
{
    printf("========== RANKLAB ==========\n");
    printf("Central de Ordenacao Inteligente\n");
    printf("Pontuacoes inteiras entre 0 e 100.\n");
}

int lerInteiro(const char mensagem[], int minimo, int maximo)
{
    char linha[128];
    char *fim;
    long valor;

    while (1) {
        printf("%s", mensagem);
        fflush(stdout);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            fprintf(stderr, "\nEntrada encerrada antes do esperado.\n");
            exit(EXIT_FAILURE);
        }

        if (strchr(linha, '\n') == NULL && !feof(stdin)) {
            int c;

            while ((c = getchar()) != '\n' && c != EOF) {
                /* Descarta o restante da linha muito longa. */
            }

            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }

        errno = 0;
        valor = strtol(linha, &fim, 10);

        if (fim != linha) {
            while (isspace((unsigned char)*fim)) {
                fim++;
            }

            if (*fim == '\0' &&
                errno != ERANGE &&
                valor >= minimo &&
                valor <= maximo) {
                return (int)valor;
            }
        }

        printf(
            "Entrada invalida. Digite um inteiro entre %d e %d.\n",
            minimo,
            maximo
        );
    }
}

void lerPontuacoes(int v[], int n)
{
    int i;
    char mensagem[64];

    for (i = 0; i < n; i++) {
        snprintf(
            mensagem,
            sizeof mensagem,
            "Pontuacao %d de %d: ",
            i + 1,
            n
        );

        v[i] = lerInteiro(mensagem, 0, 100);
    }
}

void gerarPontuacoes(int v[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        v[i] = rand() % 101;
    }
}

void copiarVetor(int origem[], int destino[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

void exibirVetor(int v[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        printf("%s%d", i == 0 ? "" : " ", v[i]);
    }

    printf("\n");
}

void exibirAmostra(int v[], int n)
{
    exibirVetor(v, n <= TAM ? n : TAM);

    if (n > TAM) {
        printf(
            "(Exibidos apenas os primeiros %d de %d valores.)\n",
            TAM,
            n
        );
    }
}

int foraDeOrdem(int a, int b, int ordem)
{
    return ordem == 1 ? a > b : a < b;
}

Resultado bubbleDetalhado(int v[], int n, int ordem)
{
    Resultado r = {0, 0, 0.0};
    int i, j, aux;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            r.comparacoes++;

            if (foraDeOrdem(v[j], v[j + 1], ordem)) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;

                r.movimentos++;
            }
        }
    }

    return r;
}

Resultado selectionDetalhado(int v[], int n, int ordem)
{
    Resultado r = {0, 0, 0.0};
    int i, j, posicao, aux;

    for (i = 0; i < n - 1; i++) {
        posicao = i;

        for (j = i + 1; j < n; j++) {
            r.comparacoes++;

            if (foraDeOrdem(v[posicao], v[j], ordem)) {
                posicao = j;
            }
        }

        if (posicao != i) {
            aux = v[i];
            v[i] = v[posicao];
            v[posicao] = aux;

            r.movimentos++;
        }
    }

    return r;
}

Resultado insertionDetalhado(int v[], int n, int ordem)
{
    Resultado r = {0, 0, 0.0};
    int i, j, chave;

    for (i = 1; i < n; i++) {
        chave = v[i];
        j = i - 1;

        while (j >= 0) {
            r.comparacoes++;

            if (!foraDeOrdem(v[j], chave, ordem)) {
                break;
            }

            v[j + 1] = v[j];
            r.movimentos++;
            j--;
        }

        v[j + 1] = chave;
    }

    return r;
}

Resultado ordenar(int v[], int n, int metodo, int ordem)
{
    Resultado r;
    clock_t inicio, fim;

    inicio = clock();

    if (metodo == 1) {
        r = bubbleDetalhado(v, n, ordem);
    } else if (metodo == 2) {
        r = selectionDetalhado(v, n, ordem);
    } else {
        r = insertionDetalhado(v, n, ordem);
    }

    fim = clock();

    r.segundos = (double)(fim - inicio) / CLOCKS_PER_SEC;

    return r;
}


int bubbleSort(int v[], int n)
{
    return bubbleDetalhado(v, n, 1).comparacoes;
}

int selectionSort(int v[], int n)
{
    return selectionDetalhado(v, n, 1).comparacoes;
}

int insertionSort(int v[], int n)
{
    return insertionDetalhado(v, n, 1).comparacoes;
}

int vetoresIguais(int a[], int b[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}