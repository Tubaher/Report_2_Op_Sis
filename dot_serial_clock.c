/******************************************************************************
* FILE: dotprod_serial.c
* DESCRIPTION:
*   This is a simple serial program which computes the dot product of two
*   vectors.  The threaded version can is dotprod_mutex.c.
* SOURCE: Vijay Sonnad, IBM
* LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION  1E9
/*
The following structure contains the necessary information
to allow the function "dotprod" to access its input data and
place its output so that it can be accessed later.
*/

typedef struct
{
    double      *a;
    double      *b;
    double     sum;
    int    veclen;
} DOTDATA;

DOTDATA dotstr;
struct timespec requestStart, requestEnd;
double tiempo;
/*
We will use a function (dotprod) to perform the scalar product.
All input to this routine is obtained through a structure of
type DOTDATA and all output from this function is written into
this same structure.  While this is unnecessarily restrictive
for a sequential program, it will turn out to be useful when
we modify the program to compute in parallel.
*/

void dotprod()
{

/* Define and use local variables for convenience */

    int start, end, i;
    double mysum, *x, *y;

    start=0;
    end = dotstr.veclen;
    x = dotstr.a;
    y = dotstr.b;

/*
Perform the dot product and assign result
to the appropriate variable in the structure.
*/
    clock_gettime(CLOCK_REALTIME, &requestStart);
    mysum = 0;
    for (i=start; i<end ; i++)
    {
        mysum += (x[i] * y[i]);
    }
    dotstr.sum = mysum;
    clock_gettime(CLOCK_REALTIME, &requestEnd);
    tiempo = ( requestEnd.tv_sec - requestStart.tv_sec )
                   + ( requestEnd.tv_nsec - requestStart.tv_nsec )
                     / BILLION;
    //printf( "%lf\n", tiempo );


}

/*
The main program initializes data and calls the dotprd() function.
Finally, it prints the result.
*/

double dot_serial_clock(int dimension_arreglo) {
    int i;
    double *a, *b;

/* Assign storage and initialize values */
    a = (double *) malloc(dimension_arreglo * sizeof(double));
    b = (double *) malloc(dimension_arreglo * sizeof(double));

    for (i = 0; i < dimension_arreglo; i++) {
        a[i] = 1;
        b[i] = a[i];
    }

    dotstr.veclen = dimension_arreglo;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;


    dotprod(); /* Perform the  dotproduct */ //Medirle con get time

/* Print result and release storage */
    //printf("Sum =  %f \n", dotstr.sum);

    return tiempo;
}
