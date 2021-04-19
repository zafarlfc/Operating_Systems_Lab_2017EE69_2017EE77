#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int fib[1000];
int n;

void *fibonacci(void *args)
{
	int i;
	
	fib[0] = 0;
	fib[1] = 1;
	
	for (i = 2; i < n; i++)
	{
		fib[i] = fib[i-1] + fib[i-2];
	}
	
	pthread_exit(0);
}

int main()
{
	int j;
	
	printf("Enter the number of terms to be generated: ");
	scanf("%d", &n);
	
	pthread_t tid;      
         
        pthread_create(&tid, NULL, fibonacci, NULL);
        pthread_join(tid, NULL);
	
        printf("Fibonnaci sequence is:\n");
        
        for (j = 0; j < n; j++)
        {
        	printf("%d, ", fib[j]);
        }
        printf("\n");
        
        return 0;       
}
