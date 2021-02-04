# Proyecto Final Sistemas Operativos


## Integrantes
### Diego Eduardo García Díaz A01747264
### Abraham Alejandro Sáenz Tirado A00823861
### Carlos Andrés Luna Leyva A00826148


## Objetivo


En este archivo se encuentra la documentación referente al proyecto final de sistemas operativos. Donde se describe paso a paso la tecnología, conceptos aprendidos, propósito del programa y manual de usuario.


## Tecnología


El proyecto se realizó en C++, donde hicimos uso de **Threads** y **Semáforos** para simular diferentes cargas de información. Donde se pueda apreciar el comportamiento y las ventajas de utilizar semáforos. 


## Código


Se crearon tres secciones diferentes, referentes a 3 ejercicios independientes. Donde cada uno se encarga de manejar diferentes tipos de procesos. Y que se describen a continuación:


| Sección       | Referencia       | Responsable |
| ------------- |:-------------:|:-----------:|
| Calculo de PI | Ejercicio Random    | Abraham Alejandro|
| Moléculas H2O | Ejercicio 9     |  Carlos Luna |
| Producción y Consumo de Datos | Ejercicio 7    |  Diego García|




### Procesos


* **Calculo de PI** - Calculo de pi utilizando rectángulos y dividiendo el calculo con threads y semáforos.
* **Moléculas H2O** - Ejecución de dos tipos de procesos: 
    * Uno para generar oxígeno
    * Otro para generar hidrógeno.
    * Sincronizando los procesos de tal manera que se haya ejecutado un proceso de oxígeno y 2 de hidrógeno para poder formar una molécula de agua.
* **Producción y Consumo de Datos** - Ejecución de dos procesos concurrentes que cooperan como productor y consumidor con las siguientes restricciones:
    * El productor siempre produce un dato cada vez
    * El consumidor debe tomar de dos datos a la vez.
    * Suponiendo que el buffer común es de tamaño 2. 
    
### Manual de Usuario


1. Se compila y ejecuta el programa. 
1. Se selecciona que programa ejecutar de los tres disponibles.
1. Se ingresan los inputs requeridos por cada sección en específico. 
1. Se repite el proceso con cualquier otro programa disponible en el menú.