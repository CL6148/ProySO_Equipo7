#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define MAX_NUM_THREADS 32000

sem_t sH;                           // Semaforo de Hidrogeno
sem_t sA;                           // Semaforo de Agua

//variables globales
int hidrogeno;
int oxigeno;
int molAgua;

void func_union() {
    molAgua++;                      // La funcion de union resta las cantidades apropiadas de las variables globales
                                    // y agrega 1 a la cantidad de moleculas de agua.

    sem_post(&sA);                  // Al finalizar, regresa el semaforo de agua 2 veces para que los proces de hidrogeno
    sem_post(&sA);                  // utilizados puedan finalizar.
}

void *proc_genH(void*) {
    sem_post(&sH);                  // Incrementa el semaforo de hidrogeno por 1 al igual que la variable global de hidrogeno.
    hidrogeno++;

    sem_wait(&sA);                  // Espera hasta recibir señal del semaforo de agua para finalizar, indicando que la funcion
    hidrogeno--;                    // de union se ejecuto correctamente (2H + O) y cambia el valor global de hidrogeno.
}

void *proc_genO(void*) {
    oxigeno++;                      // Aumenta la variable global de oxigeno por 1.

    sem_wait(&sH);                  // Espera hasta recibir 2 señales del semaforo de hidrogeno para poder ejectuar la
    sem_wait(&sH);                  // funcion de union correctamente.
    func_union();
    oxigeno--;
}

int main()
{
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
    return 0;
}
