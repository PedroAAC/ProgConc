//Lab1 ProgConc

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

//função executada pelas threads
void* say_colorado(void* arg){
    long int id =(long int) arg;
    printf("I'M A GIRAFFE ~~~~~~ tid = %ld\n", id);
    pthread_exit(NULL);
}

//função principal
int main(int argc, char* argv[]){
    //declarar variáveis
    int nthreads;
    pthread_t *tid;

    //receber argumentos
    if(argc<2){
        printf("ERRO de entrada: digite %s <numero de threads>\n", argv[0]);
        return 1;
    }
    
    //inicializar as variáveis
    nthreads = atoi(argv[1]);
    tid = (pthread_t*) malloc(nthreads*sizeof(pthread_t));
    if(tid==NULL){
        printf("ERRO de alocaçâo de memória\n");
        return 2;
    }

    //criar as threads
    for(int i=0; i<nthreads; i++){
        pthread_create(&tid[i], NULL, say_colorado, (void*)i);
    }

    //esperar as threads terminarem
    for(int i=0; i<nthreads; i++){
        pthread_join(tid[i], NULL);
    }
    
    //finalizar o programa
    printf("FIM\n");
    return 0;
}
