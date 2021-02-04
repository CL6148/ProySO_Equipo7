#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <iostream>
using namespace std;

//Variables globales
dispatch_semaphore_t semaforoDisponibles;
dispatch_semaphore_t semaforoOcupado;
int datosProducidos;
int numeroHilos;

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

//Proceso principal
int main(void){

    datosProducidos = 0;
    numeroHilos = 0;

    pthread_t thread_1, thread_2;

    //inicializacion de semaforos
    semaforoDisponibles = dispatch_semaphore_create(2);
    semaforoOcupado = dispatch_semaphore_create(0);

    cout << "Ingresar el numero de veces que se repetira el proceso (mayor a 1): ";
    cin >> numeroHilos;

    pthread_create(&thread_1, NULL, *procesoProductor, NULL);
    pthread_create(&thread_2, NULL, *procesoConsumidor, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    
    return 0;
}