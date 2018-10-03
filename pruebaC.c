// Realice varias gráficas para valores diferentes del número de hilos. En cada gráfica el eje X representa la
// dimensión de los arreglos y el Y el tiempo de ejecución de la sección del algoritmo 1. Para tomar cada
// punto de estas gráficas usted debe tomar el promedio de al menos 10 ejecuciones.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"
#define EJECUCIONES 10
#define NODOS 4
#define LEN 100000

int main(int argc, char const *argv[]) {
  //-------------------------------
  //Tiempo serial
  double tiempo_acu;

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_acu = tiempo_acu + dot_serial_clock(LEN);
  }

  printf("Tiempo promedio Serial 10 ejecuciones: %f\n",tiempo_acu/(double)EJECUCIONES);

  //-------------------------------
  //Tiempo pthread
  double tiempo_acu2;

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_acu2 = tiempo_acu2 + dot_mutex_clock(NODOS,LEN);
  }

  printf("Tiempo promedio Pthread 10 ejecuciones: %f\n",tiempo_acu2/(double)EJECUCIONES);

  //------------------------------
  //Tiempo openmp
  double tiempo_acu3;

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_acu3 = tiempo_acu3 + dotprod_openmp_clock(NODOS,LEN);
  }

  printf("Tiempo promedio OpenMP 10 ejecuciones: %f\n",tiempo_acu3/(double)EJECUCIONES);

  return 0;
}
