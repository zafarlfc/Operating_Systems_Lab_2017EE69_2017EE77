#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

double x, y, z, pi;
int i, count, points; 
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

void *worker_thread(void* args)
{
      x = (double)rand()/RAND_MAX;
      y = (double)rand()/RAND_MAX;
      z = x*x + y*y;
      
      if (z <= 1) // the point fall within the circle
      { 
      	count++;
      } 
     pthread_exit(0);
}


int main()
{
	printf("Enter the No. of points to be generated:  ");
	scanf("%d",&points);
	
	//Defining an array of threads	
	pthread_t tid[points];
	  
     	for ( i = 0; i <= points; i++) 
     	{
		pthread_mutex_lock(&lock);	
	   	pthread_create(&tid[i], NULL, worker_thread, NULL);
	   	pthread_join(tid[i], NULL);
	   	pthread_mutex_unlock(&lock);
   	}
 
	pi = (double)4*count/(double)points;  
	
	printf("The Estimated value of pi = %lf \n", pi); 	   	
   	
   	return 0;
}

