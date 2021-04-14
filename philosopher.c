//To compile: gcc philosopher.c -o test -pthread
//To run    : ./test 5 1 1

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#define LEFT		(philosopher + number_philosophers - 1) % number_philosophers
#define RIGHT 	 	(philosopher + 1) % number_philosophers
#define THINKING 	0
#define HUNGRY		1
#define EATING 		2
#define UPDATE_DELAY 	200

int number_philosophers;
int time_thinking;
int time_eating;

int* state;

sem_t mutex;
sem_t* vector_mutex;

pthread_t* thread_philosopher;
pthread_t thread_impressora;

void* Philosopher ();
void* Printer();
void take_forks (int);
void put_forks (int);
void test (int);
void think(int);
void eat(int);


int main (int argc, char* argv[]) {
  
	if(argc < 4) {
  	printf("usage: ./test number_philosophers time_thinking time_eating\n");
    return 1;
  }
  
  sscanf(argv[1],"%d", &number_philosophers);
  sscanf(argv[2],"%d", &time_thinking);
  sscanf(argv[3],"%d", &time_eating);
  
  vector_mutex = malloc(sizeof(sem_t) * number_philosophers);
  thread_philosopher = malloc(sizeof(pthread_t) * number_philosophers);
  state = malloc(sizeof(int) * number_philosophers);
    
  sem_init(&mutex, 0, 1);
  
  
  pthread_create(&thread_impressora, NULL, &Printer, NULL );
  
  int error;
  for (int i = 0; i < number_philosophers; i++) {
    sem_init(&(vector_mutex[i]), 0, 0);    // Initializing semaphores
    
    error = pthread_create(&(thread_philosopher[i]), NULL, &Philosopher, NULL);
    
    if (error != 0) {
      printf("Se ferrou!!! Can't create thread: [%s]\n", strerror(error));
      return 1;
    }
    
    state[i] = HUNGRY;    // Initializing states, all hungry
	}
  
  pthread_join(thread_impressora, NULL);    
  
  for (int i = 0; i < number_philosophers; i++) {
  	pthread_join(thread_philosopher[i], NULL);    // Executing philosophers 
  }
  
  
  
  
  // finalzinho

  sem_destroy(&mutex);
  
  for (int i = 0; i < number_philosophers; i++) {		
    sem_destroy(&vector_mutex[i]);    // Destroying semaphore vector
  }  
  
  return 0;
}


void* Philosopher () {	  // philosopher: philosopher number, from 0 to number_philosophers - 1
  
  pthread_t currThread = pthread_self();
  
  int i;
  for (i = 0; i < number_philosophers; i++) {
  	if (thread_philosopher[i] == currThread) {
      break;
    }
  }
  //printf("thread: %d\n", i);
  
  int philosopher = i;
  
  
  while (1) {
    think(philosopher);	                // philosopher is thinking
    take_forks(philosopher);            // acquire two forks or block if it's not, nobody will eat
    eat(philosopher);                   // spaghetti
    put_forks(philosopher);             // put both fork back on the table
  }
}

void* Printer(){
  while(1) {
  	sem_wait(&mutex);
    
    printf("________________________________________________________");
    printf("\n\n\n");
    for(int i = 0; i < number_philosophers; i++)
    {
      
    	if(state[i] == EATING)
        printf("E ");
      
    	else if(state[i] == HUNGRY)
        printf("H ");
      
      else if(state[i] == THINKING)
        printf("T ");
    }
    printf("\n\n\n");
    sem_post(&mutex);
    usleep(UPDATE_DELAY * 1000);
  }
  
}

void take_forks (int philosopher) {     // philosopher: philosopher number, from 0 to number_philosophers - 1
  
  sem_wait(&mutex);                     // enter critical region
  state[philosopher] = HUNGRY;          // record fact that philosopher i is hungry
  test(philosopher);                    // try to acquire 2 forks
  sem_post(&mutex);                     // exit critical region
  sem_wait(&vector_mutex[philosopher]); // block if forks were not acquired
}

void put_forks (int philosopher) {      // philosopher: philosopher number, from 0 to number_philosophers - 1

  sem_wait(&mutex);                     // enter critical region
  state[philosopher] = THINKING;        // philosopher has finished eating
  test(LEFT);                           // see if left neighbor can now eat
  test(RIGHT);                          // see if right neighbor can now eat
  sem_post(&mutex);                     // exit critical region
}

void test (int philosopher) {           // philosopher: philosopher number, from 0 to number_philosophers - 1
  
  if (state[philosopher] == HUNGRY && 
      state[LEFT] != EATING && 
      state[RIGHT] != EATING) {
        
    state[philosopher] = EATING;
    sem_post(&vector_mutex[philosopher]);
  }
}

void think (int philosopher) {
  sleep(time_thinking);
}

void eat (int philosopher) {
  sleep(time_eating);
}
