#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size
int remaining = 150;
int initial = 0; 

void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node *new_node = new node();
	new_node->id = thread_id;
	new_node->size = size;
	myqueue.push(*new_node);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	while(true)
	{ 
		while (!myqueue.empty()) 
		{
			node *incoming = new node();
			incoming = &myqueue.front();
			myqueue.pop();
			if(incoming->size <= MEMORY_SIZE-initial)
			{
				thread_message[incoming->id] = initial;
				remaining = remaining - incoming->size;
			}
			else
			{
                thread_message[incoming->id] = -1;
            }
			sem_post(&semlist[incoming->id]);
		}
		if(remaining == 0)
			break;
	}
	return 0;
}

void * thread_function(void * id) 
{
	pthread_mutex_lock(&sharedLock);
	//This function will create a random size, and call my_malloc
	//Block
	//Then fill the memory with id's or give an error prompt
	int min = 1;
	int max = 150/6;
	int size = rand() %(max-min+1)+min;
	//Pointer p points to the memory address of id
	int *p = (int*)id;
	my_malloc(*p,size);
	//Down semaphore and wait until server function ups semaphore
	sem_wait(&semlist[*p]);

	if(thread_message[*p] == -1)
	{
		cout << "For thread " << *p << ", there is not enough memory" << endl;
	}
	else
	{
		for(int i=initial; i<size+initial; i++)
		{
			if(*p == 0)
				memory[i] = '0';
			else if(*p == 1)
				memory[i] = '1';
			else if(*p == 2)
				memory[i] = '2';
			else if(*p == 3)
				memory[i] = '3';
			else if(*p == 4)
				memory[i] = '4';
			else if(*p == 5)
				memory[i] = '5';
			else if(*p == 6)
				memory[i] = '6';
			else if(*p == 7)
				memory[i] = '7';
			else if(*p == 8)
				memory[i] = '8';
			else if(*p == 9)
				memory[i] = '9';								
		}
		initial+= size;
	}
	pthread_mutex_unlock(&sharedLock);
	return 0;
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{
		sem_init(&semlist[i],0,0);
	}

	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{
		char zero = '0'; 
		memory[i] = zero;
	}

   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
	// You need to print the whole memory array here.
	cout << "Dumping the memory:"<<endl;

	for (int i = 0; i < MEMORY_SIZE; ++i)
    {
        cout<< memory[i];
    }
}

int main (int argc, char *argv[])
 {

 	//You need to create a thread ID array here
	int thread_id[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i)
     {
		 thread_id[i] = i;
	 }

 	init();	// call init
 	//You need to create threads with using thread ID array, using pthread_create()
	pthread_t thread_array[NUM_THREADS];

	for(int i=0; i < NUM_THREADS; i++)
	{
		pthread_create(&thread_array[i],NULL,thread_function,(void *) &thread_id[i]);
	}

 	//You need to join the threads
	for (int i = 0; i < NUM_THREADS; i++)
	{
        pthread_join(thread_array[i], NULL);
	}

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }