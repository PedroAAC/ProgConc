#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#define N 5

FILE *fptr;
char bfr[N];

void *LeituraF (void *arg){
    long int id = (long int) arg;
    fptr = fopen("ext.txt", "r");
    fseek(fptr, 0L, SEEK_END);
    long int size = ftell(fptr);
    size = (size-1)/5;

    for(int i = 0; i<size; i++){
        fgets (bfr, 1, fptr);
        printf("%s", &bfr[0]);
    }

    fclose(fptr);
}

int main(int argc, char *argv[]) {
    
    pthread_t *tid;
    
    tid = (pthread_t*) malloc(sizeof(pthread_t));
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}

    if (pthread_create(&tid[0], NULL, LeituraF, 0)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    for(int i=0; i<N; i++){
        printf("%s", &bfr[i]);
    }

}
