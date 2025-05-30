#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Buffer circular
long long int *buffer;
int N, M, C;
int count = 0;
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t vazio = PTHREAD_COND_INITIALIZER;

int total_primos = 0;
int *primos_por_consumidor;
int numeros_gerados = 0;
int numeros_consumidos = 0;

// Função de verificação de primalidade
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Thread produtora
void* produtor(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (numeros_gerados >= N) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (count == M) {
            pthread_cond_wait(&cheio, &mutex);
        }

        long long int valor = numeros_gerados + 1; // Sequência de 1 a N
        buffer[in] = valor;
        in = (in + 1) % M;
        count++;
        numeros_gerados++;

        pthread_cond_signal(&vazio);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Thread consumidora
void* consumidor(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (numeros_consumidos >= N && count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (count == 0 && numeros_consumidos < N) {
            pthread_cond_wait(&vazio, &mutex);
        }

        if (numeros_consumidos >= N && count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        long long int valor = buffer[out];
        out = (out + 1) % M;
        count--;
        numeros_consumidos++;

        pthread_cond_signal(&cheio);
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(valor)) {
            pthread_mutex_lock(&mutex);
            total_primos++;
            primos_por_consumidor[id]++;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

// Função principal
int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s N M C\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    C = atoi(argv[3]);

    if (N <= 0 || M <= 0 || C <= 0) {
        fprintf(stderr, "Erro: N, M e C devem ser inteiros positivos.\n");
        return 1;
    }

    buffer = malloc(M * sizeof(long long int));
    primos_por_consumidor = calloc(C, sizeof(int));

    pthread_t prod;
    pthread_t cons[C];
    int ids[C];

    pthread_create(&prod, NULL, produtor, NULL);

    for (int i = 0; i < C; i++) {
        ids[i] = i;
        pthread_create(&cons[i], NULL, consumidor, &ids[i]);
    }

    pthread_join(prod, NULL);

    for (int i = 0; i < C; i++) {
        pthread_join(cons[i], NULL);
    }

    // Identifica a thread vencedora
    int vencedora = 0;
    for (int i = 1; i < C; i++) {
        if (primos_por_consumidor[i] > primos_por_consumidor[vencedora]) {
            vencedora = i;
        }
    }

    printf("Total de primos encontrados: %d\n", total_primos);
    printf("Thread vencedora: Consumidor #%d com %d primos\n", vencedora, primos_por_consumidor[vencedora]);

    free(buffer);
    free(primos_por_consumidor);
    return 0;
}
