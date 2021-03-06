/*****************************************************************************
* FILE: dotprod_mutex.c
* DESCRIPTION:
*   This example program illustrates the use of mutex variables
*   in a threads program. This version was obtained by modifying the
*   serial version of the program (dotprod_serial.c) which performs a
*   dot product. The main data is made available to all threads through
*   a globally accessible  structure. Each thread works on a different
*   part of the data. The main thread waits for all the threads to complete
*   their computations, and then it prints the resulting sum.
* SOURCE: Vijay Sonnad, IBM
* LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define BILLION  1E9
/*
The following structure contains the necessary information
to allow the function "dotprod" to access its input data and
place its output into the structure.  This structure is
unchanged from the sequential version.
*/

typedef struct
 {
   double      *a;
   double      *b;
   double     sum;
   int     veclen;
 } DOTDATA;

struct timespec requestStart, requestEnd;
double tiempo2 = 0;
/* Define globally accessible variables and a mutex */

   DOTDATA dotstr;
   pthread_mutex_t mutexsum;


/*
The function dotprod is activated when the thread is created.
As before, all input to this routine is obtained from a structure
of type DOTDATA and all output from this function is written into
this structure. The benefit of this approach is apparent for the
multi-threaded program: when a thread is created we pass a single
argument to the activated function - typically this argument
is a thread number. All  the other information required by the
function is accessed from the globally accessible structure.
*/

void *dotprodMutex(void *arg)
{

/* Define and use local variables for convenience */

   int i, start, end, len ;
   long offset;
   double mysum, *x, *y;
   offset = (long)arg;

   len = dotstr.veclen;
   start = offset*len;
   end   = start + len;
   x = dotstr.a;
   y = dotstr.b;

/*
Perform the dot product and assign result
to the appropriate variable in the structure.
*/ clock_gettime(CLOCK_REALTIME, &requestStart);
   mysum = 0;
   for (i=start; i<end ; i++)
    {
      mysum += (x[i] * y[i]);
    }
  clock_gettime(CLOCK_REALTIME, &requestEnd);
  //Tiempo temporal
  double temp = ( requestEnd.tv_sec - requestStart.tv_sec )
                 + ( requestEnd.tv_nsec - requestStart.tv_nsec )
                   / BILLION;
/*
Lock a mutex prior to updating the value in the shared
structure, and unlock it upon updating.
*/
   pthread_mutex_lock (&mutexsum);
   if(tiempo2 < temp) tiempo2 = temp; //Tomamos el tiempo del hilo con mayor duración
   //printf("%lf\n", tiempo2);
   //printf("%lf\n", temp);
   dotstr.sum += mysum;
   //printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n",offset,start,end,mysum,dotstr.sum);
   pthread_mutex_unlock (&mutexsum);
   pthread_exit((void*) 0);

}

/*
The main program creates threads which do all the work and then
print out result upon completion. Before creating the threads,
The input data is created. Since all threads update a shared structure, we
need a mutex for mutual exclusion. The main thread needs to wait for
all threads to complete, it waits for each one of the threads. We specify
a thread attribute value that allow the main thread to join with the
threads it creates. Note also that we free up handles  when they are
no longer needed.
*/

double dot_mutex_clock(int num_hilos, int dimension_arreglo)
{

pthread_t callThd[num_hilos];
long i;
double *a, *b;
void *status;
pthread_attr_t attr;

/* Assign storage and initialize values */

a = (double*) malloc (dimension_arreglo*sizeof(double));
b = (double*) malloc (dimension_arreglo*sizeof(double));

for (i=0; i<dimension_arreglo; i++) {
  a[i]=1;
  b[i]=a[i];
  }

dotstr.veclen = dimension_arreglo/num_hilos;
dotstr.a = a;
dotstr.b = b;
dotstr.sum=0;

pthread_mutex_init(&mutexsum, NULL);

/* Create threads to perform the dotproduct  */  //Desde aquí
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

for(i=0;i<num_hilos;i++)
  {
  /* Each thread works on a different set of data.
   * The offset is specified by 'i'. The size of
   * the data for each thread is indicated by VECLEN.
   */
   pthread_create(&callThd[i], &attr, dotprodMutex, (void *)i);
   }

pthread_attr_destroy(&attr);
/* Wait on the other threads */

for(i=0;i<num_hilos;i++) {
  pthread_join(callThd[i], &status);
  }
/* After joining, print out the results and cleanup */
//Hasta Aquí
//printf ("Sum =  %f \n", dotstr.sum);
//printf( "%lf\n", tiempo2 );

free (a);
free (b);
pthread_mutex_destroy(&mutexsum);
return tiempo2; // Se podria evitar el problema de hilos sueltos
                //empleado un puntero.
pthread_exit(NULL);

}
