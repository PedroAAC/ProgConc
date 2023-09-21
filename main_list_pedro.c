#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"
#include "lock.h" // Inclua o cabeçalho da biblioteca lock.h

#define QTDE_OPS 10000000
#define QTDE_INI 100
#define MAX_VALUE 100

struct list_node_s* head_p = NULL;
int nthreads;

void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read;
   in = out = read = 0;

   for (long int i = id; i < QTDE_OPS; i += nthreads) {
      op = rand() % 100;
      if (op < 98) {
         // Substitua read_lock() por read_lock(&rwlock)
         read_lock(&rwlock);
         Member(i % MAX_VALUE, head_p);
         // Substitua read_unlock() por read_unlock(&rwlock)
         read_unlock(&rwlock);
         read++;
      } else if (98 <= op && op < 99) {
         // Substitua write_lock() por write_lock(&rwlock)
         write_lock(&rwlock);
         Insert(i % MAX_VALUE, &head_p);
         // Substitua write_unlock() por write_unlock(&rwlock)
         write_unlock(&rwlock);
         in++;
      } else if (op >= 99) {
         // Substitua write_lock() por write_lock(&rwlock)
         write_lock(&rwlock);
         Delete(i % MAX_VALUE, &head_p);
         // Substitua write_unlock() por write_unlock(&rwlock)
         write_unlock(&rwlock);
         out++;
      }
   }

   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   pthread_t* tid;
   double ini, fim, delta;

   if (argc < 2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   for (int i = 0; i < QTDE_INI; i++)
      Insert(i % MAX_VALUE, &head_p);

   GET_TIME(ini);

   tid = malloc(sizeof(pthread_t) * nthreads);
   if (tid == NULL) {
      printf("--ERRO: malloc()\n");
      return 2;
   }

   // Inicialize a biblioteca lock.h
   init_lock();

   for (long int i = 0; i < nthreads; i++) {
      if (pthread_create(tid + i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n");
         return 3;
      }
   }

   for (int i = 0; i < nthreads; i++) {
      if (pthread_join(*(tid + i), NULL)) {
         printf("--ERRO: pthread_join()\n");
         return 4;
      }
   }

   // Destrua a biblioteca lock.h
   destroy_lock();

   GET_TIME(fim);
   delta = fim - ini;
   printf("Tempo: %lf\n", delta);

   free(tid);
   Free_list(&head_p);

   return 0;
}
