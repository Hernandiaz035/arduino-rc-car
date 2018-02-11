# Arduino RC Car

Implementacion de un carro de control remoto como proyecto de Aula de la asignatura *Electronica Digital y Microcontroladores*.

El software controlador del carro cuenta con:
* Implementacion primitiva de **Multitasking**, esto mediante la funcion `millis()` y deltas de tiempo predefinidos.
* Estructura de filtro 2D de segundo orden diseñado mediante la funcion `Butterworth` en **Matlab**, esta tiene como fin suavizar la aceleracion del carro que debido a que la comunicacion no verifica fallos de comunicacion, este puede generar cambios bruscos.
* Comunicacion Empaquetada de Variables, en este caso, solo aceleracion y direccion, mediante codificacion ASCII (Este empaqueramiento es semejante a **JSON**).


En este proyecto, se requiere una comunicacion en tiempo real de las acciones a ejecutar y la perdida de paquetes de informacion no son un problema (**NOACK**), por lo cual esta es unidireccional (**SIMPLEX**)

El Hardware controlado tiene un Puente H que permite Retroceder, por lo cual el valor de aceleracion puede ser negativo.

En el Hardware controlado, el servo tiene un rango definido para el angulo de la direccion, este esta dentro de los `#DEFINE` del software


El control de carro envia mediante Serial la informacion empaquetada de las lecturas Analogicas, en caso de que un poteciometro auxiliar (*yaw*) esté sobre un valor, este envia valores negativos de aceleracion con el fin de retroceder


## Hardware necesario

### Carro
* Arduino (cualquiera)
* Puente H
* Bateria
* Servo
* MotoReductor
* Modulo de comunicacion UART (una opcion viable y de bajo coste es un modulo de bluetooth, **HC-05**)

### Control
* PIC16F1823 (Programado en PICC de CCS)
* 3 Potenciometros para las Lecturas Analogicas
* LEDs Indicadores de flujo del programa
* Modulo de Comunicacion UART
* Bateria y Regulador de Voltaje segun tensiones de operacion del subsistema.