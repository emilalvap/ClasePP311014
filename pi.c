/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <omp.h>
static long num_steps = 10000000;
double step;
int pi_critical ()
{
	  int i;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;
	  start_time = omp_get_wtime();

#pragma omp parallel for shared(sum) private(x) 
	  for (i=1;i<= num_steps; i++){
		  x = (i-0.5)*step;
#pragma omp critical (sumlock)
		  sum = sum + 4.0/(1.0+x*x);
	  }

	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %f in %f seconds\n ",num_steps,pi,run_time);
}	 

 int pi_atomic ()
{
	  int i;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;
	  start_time = omp_get_wtime();

#pragma omp parallel for reduction(+:sum) private(x)
	  for (i=1;i<= num_steps; i++){
		  x = (i-0.5)*step;
#pragma omp atomic 
		  sum = sum + 4.0/(1.0+x*x);
	  }

	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %f in %f seconds\n ",num_steps,pi,run_time);
}

int pi_reduce ()
{
	  int i;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;
	  start_time = omp_get_wtime();

#pragma omp parallel for reduction(+:sum) private(x) 
	  for (i=1;i<= num_steps; i++){
		  x = (i-0.5)*step;
		  sum += 4.0/(1.0+x*x);
	  }

	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %f in %f seconds\n ",num_steps,pi,run_time);
}

int main(){
  printf("Pi reducido");
  pi_reduce();
  printf("Pi critical");
  pi_critical();
  printf("Pi atomic");
  pi_atomic();

}
// compilation using gcc and openMp libraries
// gcc -fopenmp -o x.c x
/* El comportamiento de critical y atomic es bastante parecido, consistente en establecer un semaforo de 
acceso a la variable.
En el reduce es distinto, se ejecutan todos los hilos, que tienen una copia privada de la variable
que cuando terminan todos los threads se realiza la acumulación. Este caso en particular puede originar
acumulación de errores de redondeo.
*/


