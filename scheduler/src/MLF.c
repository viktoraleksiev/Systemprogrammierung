#include "MLF.h"
#include "../include/task.h"
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue **q;
int ts;		// constant time step
int time;	// remaining time for the current task; if 0 the next is executed
int number_q;	

int init_MLF(int time_step, int num_queues)
{	// initializing the queues 
	if(time_step > 0 && num_queues > 0 ){
	q = malloc(sizeof(Queue)*num_queues);
	
        for (int i=0; i< num_queues; i++){
		q[i] = queue_new(put_in_end);
	}

   	running_task = NULL;
	ts = time_step;	
	time = 0;
	number_q = num_queues;
	
		for(int i=0; i < number_q; i++){
			if (!q[i]){
			printf("Could not allocate memory for queue.");
			exit(1);
			}
		}
	} else return -1;
        return 0;
}

void free_MLF()
{
	for(int i=0; i < number_q; i++){
   		 free(q[i]);
	}
	free(q);
}

void arrive_MLF(int id, int length)
{	// adding the new process into the frist queue
    	if (id >=0 && length > 0){
		def_task * new = malloc(sizeof(def_task));
		new->id = id;
		new->length = length;
		new->queue = 0;
		queue_offer(q[0],new);


	}
}

int zeit_scheibe(int n){
	if(n==1) return ts;
	else return zeit_scheibe(n-1)*2;
}

// finding the next process in the queues that has to be executed
def_task *find_next(){
	
	for(int i=0; i < number_q; i++){		
		if( q[i]->head != NULL ){
			if(i < number_q-1){		// in not in the last queue: caltulating time
				running_task = queue_peek(q[i]);
				time = zeit_scheibe(i+1) - 1;
				running_task->length--;
				return running_task;
			} else {			// if in the last queue (const remianing time 1 - FIFO)
				running_task = queue_peek(q[i]);
				time = 1;
				running_task->length--;
				return running_task;
			}	
		}
	}
	return NULL;
}


def_task *tick_MLF()
{	
	if (running_task == NULL){	// there is no running process before the tick		
					
				return find_next();	
	}else{

		if (time == 0 && running_task->length>0){  // the time for the last task has finished and if the process is still not ready is put in the right queue

				if(running_task->queue < number_q - 1){
					queue_poll(q[running_task->queue]);
					running_task->queue++;
					queue_offer(q[running_task->queue],running_task);
				}
				running_task = NULL;
				return find_next();

		} else 	if( time > 0 && running_task->length > 0){ // still remaining time and the process is not ready
				if(running_task->queue < number_q -1) time--;
				running_task->length--;
				return running_task;	

		} else if( time > 0 && running_task->length == 0){ // remaining time but process is ready
				def_task * old = queue_poll(q[running_task->queue]);
				free(old);
				running_task=NULL;
				return find_next();		
		
		} else {		// the process is ready right on time
			free(queue_poll(q[running_task->queue]));
			running_task = NULL;
			return find_next();
		
		}


			
	}

}

void finish_task_MLF()
{
    // TODO optional
}
