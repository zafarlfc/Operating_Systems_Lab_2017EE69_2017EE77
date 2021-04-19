#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

double x, y, z, pi;
int i, count, points; 

void *worker_thread(void* args)
{
     printf("Enter the No. of points to be generated:  ");
     scanf("%d",&points);
     
     for ( i = 0; i <= points; i++) 
     {
     
      x = (double)rand()/RAND_MAX;
      y = (double)rand()/RAND_MAX;
      z = x*x + y*y;
      
      if (z <= 1) // the point fall within the circle
      { 
      	count++;
      } 
     }
     pthread_exit(0);
}


int main()
{	
	pthread_t tid;
		
   	pthread_create(&tid, NULL, worker_thread, NULL);
   	pthread_join(tid, NULL);
 
	pi = (double)4*count/(double)points;  
	
	printf("The Estimated value of pi = %lf \n", pi); 	   	
   	
   	return 0;
}

