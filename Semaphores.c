#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include </usr/include/semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#define SHARED 1
//gcc 6277.c -o 6277 -lpthread
//./6277

const int N= 5;
int count=0,buffer[40],collect=0,tempInt=0,j=0,value=0;
sem_t syncing, full, empty,sem_counter;

//srand(time(NULL));  


void *mCounter(void*args)
{	
	int index=(intptr_t) args;
	
	pthread_t thread=pthread_self();
	printf("Counter thread %ld: received a message\n",thread);
	
     	 
	while (1){
	
         int rand= random()%(10+1);
         sleep(rand);
	
	  sem_getvalue(&sem_counter,&value);
	  if (value==0)
	  printf("Counter thread %ld: waiting to write\n",thread);
	  
          sem_wait(&sem_counter); 
          //When a mCounter thread grants access to the counter, it should add one to it.
          count++;// only operation made by counter thread.
          printf("Counter thread %ld: now adding to counter,counter value=%d\n",thread,count);
	  sem_post(&sem_counter); 
	  
  }
	
}


void *mMonitor()
{   	
	
	while (1) {
	  
	  int rand= random()%(10+1);
          sleep(rand);
	  
	  sem_getvalue(&sem_counter,&value);
	  if (value==0)
	  printf("Monitor thread: waiting to read\n\n");
	  
	  //1st critical section
	  sem_wait(&sem_counter);
	  tempInt=count;
	  printf("Monitor thread:reading a count value of=%d\n\n",tempInt);
	  count=0;//When the mMonitor thread grants access to the counter, it should reset it to 0 and save its value to use it later in buffer.
	  sem_post(&sem_counter);
	  
	  
	   sem_getvalue(&empty,&value);
	   if (value==0)
	   printf("Buffer full!!");
	   

	  //2nd critical section
	   sem_wait(&empty);//decrement number of empty spaces
	   sem_wait(&syncing);
	   printf("Monitor thread:writing to buffer at position =%d\n",buffer[j]);
	   buffer[j]=tempInt;
	   j++;
	   sem_post(&syncing); 
	   sem_post(&full);
       
} 
	
}


void *mCollector()
{
	
  	
        while (1){
        
        int rand= random()%(10+1);
	sleep(rand);
  	sem_getvalue(&full,&value);
       
	if(value==0)
	printf("nothing is in the buffer!\n\n");//if full=0, therefore empty buffer,therefore consumer will be blocked.
	
	sem_wait(&full);
	sem_wait(&syncing);	
	collect=buffer[j];
	printf("Collector thread:Reading from buffer at position =%d\n\n",buffer[j]);
	j--;//dequeue buffer one element at a time..one element/iteration.
	sem_post(&syncing);
	sem_post(&empty);
	
	
}
}


//mMonitor is the producer.
//It enqueues the value that was saved from the previous problem into the buffer.
//mCollector is the consumer it takes the data out of the buffer.


void main()
{       
	int i=0;
        printf("in main\n");
        
	pthread_t Counter[N];
	pthread_t Monitor;
	pthread_t Collector;
	
	sem_init(&syncing, SHARED, 1);
	sem_init(&full, SHARED, 0);
	sem_init(&empty, SHARED,40);
	sem_init(&sem_counter, SHARED, 1);

	
	
       for(i=0;i<=N;i++)
       {
        int *index = malloc(sizeof(int));
        *index = i; 
	pthread_create(&Counter[i], NULL, &mCounter, index);
        
       }
	pthread_create(&Monitor, NULL, &mMonitor, NULL);
	pthread_create(&Collector, NULL, &mCollector, NULL);
	
	pthread_join(Monitor,NULL);
	pthread_join(Collector,NULL);
	for(int i=0;i<10;i++){
	pthread_join(Counter[i],NULL);
        }
}
