#include <omp.h>
#include <stdlib.h>
#include<stdio.h>

int main(){
  double *array1, *array2;
  int i;
  double time1,time2;
  omp_set_nested(1);
#pragma omp parallel sections shared(array1,array2)
  {
   #pragma omp section
    { 
      time1 = omp_get_wtime();
      array1=(double*)malloc(sizeof(double)*1024*1024);
#pragma omp parallel for shared(array1) 
      for(i=0;i<1024*1024;i++)
	array1[i] = i;
      time1 = omp_get_wtime()-time1;
      printf("time nested: %f",time1);
    }
    #pragma omp section
    { time2 = omp_get_wtime();
      array2=(double*)malloc(sizeof(double)*1024*1024);
      #pragma omp parallel for shared(array2)
      for(i=0;i<1024*512;i++)
	array2[i] = i;
      time2 = omp_get_wtime()-time2;
      printf("time not nested: %f",time2);
    }
  }
}

