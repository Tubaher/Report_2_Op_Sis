// Realice varias gráficas para valores diferentes del número de hilos. En cada gráfica el eje X representa la
// dimensión de los arreglos y el Y el tiempo de ejecución de la sección del algoritmo 1. Para tomar cada
// punto de estas gráficas usted debe tomar el promedio de al menos 10 ejecuciones.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"
#define EJECUCIONES 10
#define HILOS 12
#define LEN 40000000
#include <math.h>

double calDesviacion(double media,double* tiempos){
  double acumulador;

  for(int i=0;i<EJECUCIONES;i++){
    acumulador = acumulador + ((tiempos[i]-media)*(tiempos[i]-media));
  }
  double desviacion = sqrt(acumulador/(double)EJECUCIONES);

  return desviacion;
}

int main(int argc, char const *argv[]) {
  //-------------------------------
  //Tiempo serial
  printf("  \tMedia\t Desviacion Estandar\n");
  double tiempo_acu,desv_tipica, tiempo_paso, media, desviacion;
  double tiempos[EJECUCIONES];

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_paso = dot_serial_clock(LEN);
     tiempo_acu = tiempo_acu + tiempo_paso;
     tiempos[i] = tiempo_paso;
  }

  media = tiempo_acu/(double)EJECUCIONES;
  desviacion = calDesviacion(media,tiempos);
  //printf("Tiempo promedio Serial 10 ejecuciones: %f\n",tiempo_acu/(double)EJECUCIONES);
  printf("Serial:  %f\t%f\n",media,desviacion);

  //-------------------------------
  //Tiempo pthread
  double tiempo_acu2,desv_tipica2, tiempo_paso2, media2, desviacion2;
  double tiempos2[EJECUCIONES];

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_paso2 = dot_mutex_clock(HILOS,LEN);
     tiempo_acu2 = tiempo_acu2 + tiempo_paso2;
     tiempos2[i] = tiempo_paso2;
  }

  media2 = tiempo_acu2/(double)EJECUCIONES;
  desviacion2 = calDesviacion(media2,tiempos2);
  //printf("Tiempo promedio Pthread 10 ejecuciones: %f\n",tiempo_acu2/(double)EJECUCIONES);
  printf("Pthread: %f\t%f\n",media2,desviacion2);

  //------------------------------
  //Tiempo openmp
  double tiempo_acu3,desv_tipica3, tiempo_paso3, media3, desviacion3;
  double tiempos3[EJECUCIONES];

  for(int i=0; i<EJECUCIONES; i++){
     tiempo_paso3 = dotprod_openmp_clock(HILOS,LEN);
     tiempo_acu3 = tiempo_acu3 + tiempo_paso3 ;
     tiempos3[i] = tiempo_paso3;
  }
  media3 = tiempo_acu3/(double)EJECUCIONES;
  desviacion3 = calDesviacion(media3,tiempos3);
  //printf("Tiempo promedio OpenMP 10 ejecuciones: %f\n",tiempo_acu3/(double)EJECUCIONES);
  printf("OpenMP:  %f\t%f\n",media3,desviacion3);

  return 0;
}
