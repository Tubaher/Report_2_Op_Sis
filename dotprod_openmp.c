#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#define BILLION  1E9
typedef struct
 {
   double      *a;
   double      *b;
   double     sum;
   int     veclen;
 } DOTDATA;

#define NUMTHRDS 8
#define VECLEN 10000000
DOTDATA dotstr;
struct timespec requestStart, requestEnd;
double accum;

void dotprod()
{

/* Define and use local variables for convenience */
   int start, end, len ;
   long offset;
   double mysum, *x, *y;

   len = dotstr.veclen;
   start = omp_get_thread_num()*len; //comienzo del array
   end   = start + len;              //final del array
   x = dotstr.a;
   y = dotstr.b;

/*
Perform the dot product and assign result
to the appropriate variable in the structure.
*/
   clock_gettime(CLOCK_REALTIME, &requestStart);
   mysum = 0;
   for (int i=start; i<end ; i++)
    {
      mysum += (x[i] * y[i]);
    }

  #pragma omp critical //defino la seccion critica al momento de sumar a la variable global
   dotstr.sum += mysum;
   printf ("Sum of thread %d =  %f \n", omp_get_thread_num(),dotstr.sum);
   clock_gettime(CLOCK_REALTIME, &requestEnd);

}

int main(int argc, char const *argv[]) {
  double *a, *b;
  a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
  b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));

  //Inicializo los valores de los vectores todos a uno
  for (int i=0; i<VECLEN*NUMTHRDS; i++) {
    a[i]=1;
    b[i]=a[i];
  }

  //defino los valores de la estructura global dotstr
  dotstr.veclen = VECLEN;
  dotstr.a = a;
  dotstr.b = b;
  dotstr.sum=0;

  #pragma omp parallel num_threads(NUMTHRDS)
  {
    //Ejecuto la funcion dotprod() en cada hilo
    dotprod();
    printf("Prueba de hilo %d de %d\n",omp_get_thread_num(),omp_get_num_threads());
  }

  //Imprimo la suma final
  printf ("Sum =  %f \n", dotstr.sum);
  double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
                   + ( requestEnd.tv_nsec - requestStart.tv_nsec )
                     / BILLION;
  printf( "%lf\n", accum );
  free (a);
  free (b);

  return 0;

}
