/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "threadpool.h"
//length of queue
#define QUEUE_SIZE 10
//Structure type created
struct data {
    int a;
    int b;
};
//This function adds two values
void add(void *param) {
    struct data *temp;
    temp = (struct data *)param;
    printf("I add two values %d and %d result = %d\n", temp->a, temp->b, temp->a + temp->b);
}
//main function
int main(void) {
    // thread pool initialization
    pool_init();
    //structure created for submitting work
    struct data *work = malloc(QUEUE_SIZE * sizeof(struct data));
    //run submit function upto queue length
    for(int i = 0; i != QUEUE_SIZE; ++i) {
        work[i].a = i*2; //variable *2
        work[i].b = i;  
        //if return of submit function is not 0, this means error=1 
        while(pool_submit(&add, &work[i]) != 0) { 
              
        }
    }
    
    sleep(3);
    //shutdoqn operation performed
    pool_shutdown();
    free(work);
    return 0;
}
