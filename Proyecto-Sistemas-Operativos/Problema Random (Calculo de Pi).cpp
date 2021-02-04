#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>
#include <semaphore.h>
#define MAX_NUM_THREADS 32000

//Variables globales

//Semaforo binario para asegurar que un solo un thread modifique la variable pi
sem_t lock;
double pi;
int intervals, numthreads;

//Se  declara que esta rutina se va a ejecutar a manera de thread
void * computepi(void *arg){
  int *id;
  int i;
  double localsum, width, x;

  id = (int *)arg;
  localsum = 0.0;
  width = 1.0 / intervals;
  for(i = (*id); i < intervals; i += numthreads){
    x = (i + 0.5) * width;
    localsum += (4.0 / (1.0 + x * x));
  }
  localsum *= width;

  //Se solicita el acceso a la variable pi
  sem_wait(&lock);
  //Solo 1 thread a la vez modifica a la variable pi
  pi += localsum;
  //Se habilita que otro thread pueda modificar la variable pi
  sem_post(&lock);
  pthread_exit(NULL);
}

//Inicia el programa principal
int main(){
  int ids[MAX_NUM_THREADS], i;
  //Variables para mediciones de tiempos
  struct tms buff1, buff2;
  clock_t clock1, clock2;
  float utime, stime;
  float micros;
  //Variable para almacenar los ticks del reloj por segundo
  long int ticksPerSec;

  //Inicializacion y peticion de datos
  printf("\nCaptura el numero de threads ");
  scanf("%d", &numthreads);
  printf("\nCaptura el numero de rectangulos ");
  scanf("%d", &intervals);
  
  clock1 = times(&buff1);
  pi = 0.0;
  for(i=0; i < numthreads; i++){
    ids[i] = i;
  }

  //Inicia el calculo de pi
  pthread_t hilo [numthreads];
  sem_init(&lock,0,1);

  //Se crean los threads 
  for(i=0; i < numthreads; i++){
    pthread_create (&hilo[i],NULL,computepi, &ids[i]);
  }

  //Se espera por la terminacion de los threads
  for(i=0; i < numthreads; i++){
    pthread_join (hilo[i], NULL);
  }

  clock2 = times(&buff2);

  //Imprimir los resultados
  printf("El valor de pi es: %8.7f\n", pi);
  printf("Numero de threads: %d\n", numthreads);
  micros = clock2 - clock1;
  ticksPerSec = sysconf(_SC_CLK_TCK);
  printf("Ticks por segundo = %lu\n", ticksPerSec);

  printf("Tiempo real: %10.5f segundos \n", micros/ticksPerSec);
  utime = buff2.tms_utime - buff1.tms_utime;
  printf("Tiempo de usuario: %10.5f segundos \n", utime/ticksPerSec);
  stime = buff2.tms_stime - buff1.tms_stime;
  printf("Tiempo de sistema: %10.5f segundos \n", stime/ticksPerSec);
  printf("Tiempo de ejecucion: %10.5f segundos\n", (utime + stime)/ticksPerSec);
  return 0;
}