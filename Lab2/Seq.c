#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int N;
    printf("Digite o valor de N: ");
    scanf("%d", &N);

    float *a = (float *)malloc(N * sizeof(float));
    float *b = (float *)malloc(N * sizeof(float));
    if (a == NULL || b == NULL) {
        printf("Erro de alocação de memória\n");
        return 1;
    }

    // Inicializar gerador de números aleatórios
    srand(time(NULL));

    // Gerar vetores aleatórios
    for (int i = 0; i < N; i++) {
        a[i] = (float)rand() / RAND_MAX;
        b[i] = (float)rand() / RAND_MAX;
    }

    // Calcular produto interno
    double produto = 0.0;
    for (int i = 0; i < N; i++) {
        produto += (double)(a[i] * b[i]);
    }

    // Escrever no arquivo binário
    FILE *fp = fopen("vetores.bin", "wb");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return 1;
    }

    fwrite(&N, sizeof(int), 1, fp);
    fwrite(a, sizeof(float), N, fp);
    fwrite(b, sizeof(float), N, fp);
    fwrite(&produto, sizeof(double), 1, fp);

    fclose(fp);
    free(a);
    free(b);

    printf("Arquivo gerado com sucesso!\n");
    return 0;
}
