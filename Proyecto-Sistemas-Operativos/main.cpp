#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <iostream>
#include <cstdlib>
#define MAX_NUM_THREADS 32000

using namespace std;

//Variables globales
dispatch_semaphore_t semaforoDisponibles;
dispatch_semaphore_t semaforoOcupado;
int datosProducidos = 0;
int numeroHilos = 0;
int hidrogeno;
int oxigeno;
int molAgua;

sem_t sH;                           // Semaforo de Hidrogeno
sem_t sA;                           // Semaforo de Agua

//Semaforo binario para asegurar que un solo un thread modifique la variable pi
sem_t lock;
double pi;
int intervals, numthreads;

pthread_t thread_1, thread_2;


void menu(){
  printf( "\n	Menú de opciones"
	  "\n1. Ejecutar Problema Visto en Clase 1(Ejercicio 7)"
	  "\n2. Ejecutar Problema Visto en Clase 2(Ejercicio 9)"
	  "\n3. Ejecutar Problema Problema de Sincronizacion Aleatorio"
	  "\n0. Salir"
	  "\nTeclea la opcion: ");
}

//Se  declara que esta rutina se va a ejecutar a manera de thread
void * operacionPi(void *arg){
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

void calculaPi(){
  int ids[MAX_NUM_THREADS], i;
  //Variables para mediciones de tiempos
  struct tms buff1, buff2;
  clock_t clock1, clock2;
  float utime, stime;
  float micros;
  //Variable para almacenar los ticks del reloj por segundo
  long int ticksPerSec;

  //Inicializacion y peticion de datos
  printf("\nCaptura el numero de threads (Numero Max = 32000)");
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
    pthread_create (&hilo[i],NULL,operacionPi, &ids[i]);
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
}

//Proceso Productor
static void * procesoProductor(void* arg) {

    for(int i = 0; i < numeroHilos; i++){

        // sem_wait(&semaforoDisponibles);
        dispatch_semaphore_wait(semaforoDisponibles, DISPATCH_TIME_FOREVER);
        // pthread_mutex_lock(&semaforoDisponibles);
        printf(" Se produjo 1 dato \n");
        datosProducidos++;
        // pthread_mutex_unlock(&semaforoOcupado);
        // sem_post(&semaforoOcupado);
        dispatch_semaphore_signal(semaforoOcupado);
    }
}

//Proceso Consumidor
static void * procesoConsumidor(void* arg) {

    do {
        dispatch_semaphore_wait(semaforoOcupado, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(semaforoOcupado, DISPATCH_TIME_FOREVER);

        printf(" Se consumieron 2 datos \n");

        dispatch_semaphore_signal(semaforoDisponibles);
        dispatch_semaphore_signal(semaforoDisponibles);

    } while ((datosProducidos + 1) < numeroHilos);
}

void problema7(){
  //inicializacion de semaforos
  semaforoDisponibles = dispatch_semaphore_create(2);
  semaforoOcupado = dispatch_semaphore_create(0);

  cout << "Ingresar el numero de veces que se repetira el proceso (mayor a 1): ";
  cin >> numeroHilos;

  pthread_create(&thread_1, NULL, *procesoProductor, NULL);
  pthread_create(&thread_2, NULL, *procesoConsumidor, NULL);

  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
}

void func_union() {
    molAgua++;                      // La funcion de union resta las cantidades apropiadas de las variables globales
                                    // y agrega 1 a la cantidad de moleculas de agua.

    sem_post(&sA);                  // Al finalizar, regresa el semaforo de agua 2 veces para que los proces de hidrogeno
    sem_post(&sA);                  // utilizados puedan finalizar.
}

void *proc_genH(void*) {
    sem_post(&sH);                  // Incrementa el semaforo de hidrogeno por 1 al igual que la variable global de hidrogeno.
    hidrogeno++;

    sem_wait(&sA);                  // Espera hasta recibir se�al del semaforo de agua para finalizar, indicando que la funcion
    hidrogeno--;                    // de union se ejecuto correctamente (2H + O) y cambia el valor global de hidrogeno.
}

void *proc_genO(void*) {
    oxigeno++;                      // Aumenta la variable global de oxigeno por 1.

    sem_wait(&sH);                  // Espera hasta recibir 2 se�ales del semaforo de hidrogeno para poder ejectuar la
    sem_wait(&sH);                  // funcion de union correctamente.
    func_union();
    oxigeno--;
}

void problema9(){
  hidrogeno = 0;                  // Inicializacion de variables globales en 0.
    oxigeno = 0;
    molAgua = 0;

    // Variables generales
    int i;
    int j;
    int exeH;
    int exeO;
    int idH[MAX_NUM_THREADS];
    int idO[MAX_NUM_THREADS];

    // Display Central
    cout << endl;
    cout << "Hidrogeno Disponible: " << hidrogeno << endl;
    cout << "Oxigeno Disponible: " << oxigeno << endl;
    cout << "Moleculas de Agua creadas: " << molAgua << endl;
    cout << endl;

    cout << "Cuantos procesos de Hidrogeno ejecutar? ";
    cin >> exeH;
    cout << "Cuantos procesos de Oxigeno ejecutar? ";
    cin >> exeO;

    for (i = 0; i < exeH; i++) {    //Inicializacion de identificadores para threads
        idH[i] = i;
    }
    for (j = 0; j < exeO; j++) {
        idO[j] = j;
    }

    pthread_t trdH[exeH];           //Inicializacion de threads y semaforos
    pthread_t trdO[exeO];
    sem_init(&sH, 0, 0);
    sem_init(&sA, 0, 0);

    //Ejecucion de Procesos
    for (i = 0; i < exeH; i++) {
        pthread_create(&trdH[i], NULL, proc_genH, &idH[i]);
    }
    for (j = 0; j < exeO; j++) {
        pthread_create(&trdO[j], NULL, proc_genO, &idO[j]);
    }

    //Conteo Final
    cout << endl;
    cout << "Hidrogeno Restante: " << hidrogeno << endl;
    cout << "Oxigeno Restante: " << oxigeno << endl;
    cout << "Moleculas de Agua: "  << molAgua << endl;

    system("PAUSE");
}

//Inicia el programa principal
int main(){
  //Declaracion de variables
  int iN, iOpcion;
  double dPi, dError;


  //Desplegar el menu
  menu();
  cin >> iOpcion;

  //Ciclo Centinela
  while (iOpcion !=0){

  //Ejecutar la opcion elegida por el usuario
  switch (iOpcion){
    case 1: {// Problema de Sincronizacion (Numero 7)
            problema7();
            break;
    }
    case 2: {//Problema de Sincronizacion (Numero 9)
            problema9();
            break;
    }
    case 3: {//Calculo de Pi con Rectangulos
            calculaPi();
            break;
    }
    default:
          cout << "Opcion Incorrecta\n";

  }

  //Actualizar la vcc dentro
  menu();
  cin >> iOpcion;

  }
  cout << "Adios\n";
  return 0;
}