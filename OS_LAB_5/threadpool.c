/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"
//length of queue
#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 10

#define TRUE 1

// this represents work that has to be
// completed by a thread in the pool
typedef struct {
    void (*function)(void *p);
    void *data;
}
task;

// mutex 
pthread_mutex_t lock;   // mutex lock for enqueue and dequeue
//semophore
sem_t taskc;

// the work queue
task taskQueue[QUEUE_SIZE + 1];     // one extra entry needed for determining whether the queue is full
int enqueue_count = 0;
int dequeue_count = 0;

// pthreads
pthread_t bees[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise,
int enqueue(task t) {
    pthread_mutex_lock(&lock); 
    if((enqueue_count + 1) % (QUEUE_SIZE + 1) == dequeue_count) {  // this checks if queue is full or not
        pthread_mutex_unlock(&lock);
        return 1;  //if 1 is returned, queue is full
    }
    taskQueue[enqueue_count] = t;
    enqueue_count = (enqueue_count + 1) % (QUEUE_SIZE + 1); //increment
    pthread_mutex_unlock(&lock);
    return 0;
}

// remove a task from the queue
task dequeue() {
    pthread_mutex_lock(&lock); // acquire lock before modifying the task queue
    task returned = taskQueue[dequeue_count];
    dequeue_count = (dequeue_count + 1) % (QUEUE_SIZE + 1);  //increment
    pthread_mutex_unlock(&lock);   
    return returned; 
}

// the worker thread in the thread pool
void *worker(void *param) {
    // execute the task
    task worktodo;
    while(TRUE) {
        sem_wait(&taskc); //semophore activated
        worktodo = dequeue();  //queue value stored in structure worktodo
        execute(worktodo.function, worktodo.data);  //execute function performed.
    }
}


// Executes the task provided to the thread pool
 
void execute(void (*somefunction)(void *p), void *p) {
    (*somefunction)(p);
}


// Submits work to the pool.
 
int pool_submit(void (*somefunction)(void *p), void *p) {
    int error = 0;
    task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;
    error = enqueue(worktodo);
    if(error==0) {   // success
        sem_post(&taskc);     // end semophore
    }
    return error;
}

// initialize the thread pool
void pool_init(void) {
    pthread_mutex_init(&lock, NULL); //mutex lock initiated
    sem_init(&taskc, 0, 0); //semophore initiated
    int i;
    for(i = 0; i != NUMBER_OF_THREADS; ++i) {
        pthread_create(&bees[i], NULL, worker, NULL); //pthreads created
    }
}

// shutdown the thread pool
void pool_shutdown(void) {
    int i;
    for(i = 0; i != NUMBER_OF_THREADS; ++i) {
        pthread_cancel(bees[i]);  // cancells pthread
        pthread_join(bees[i], NULL);
    }
    sem_destroy(&taskc);
    pthread_mutex_destroy(&lock);
}
