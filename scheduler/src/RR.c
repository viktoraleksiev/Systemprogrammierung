#include "RR.h"
#include "../include/task.h"
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *q;
int ts;		// constant time step
int time;	// remaining time for the current task; if 0 the next is executed

int init_RR(int time_step)
{
        q = queue_new(put_in_end);
   	running_task = NULL;
	ts = time_step;	
	time = time_step;
	if (!q){
		printf("Could not allocate memory for queue.");
		exit(1);
	}
	else return 0;
}

void free_RR()
{
    free(q);
}

void arrive_RR(int id, int length)
{
    	if (id >=0 && length > 0){
		def_task * new = malloc(sizeof(def_task));
		new->id = id;
		new->length = length;

		queue_offer(q,new);


	}
}

def_task *tick_RR()
{	
	if(running_task != NULL){
	if (time == 0){				// the time for the last task has finished 
		if(running_task->length >0){
			queue_offer(q,running_task);		
		}else free(running_task);
		running_task = queue_poll(q);
		if (running_task != NULL){
			time = ts - 1;
			running_task->length--;
		}	
		return running_task;			
	} else if( time > 0 && running_task->length > 0){	// still remaining time and the process is not ready
		time--;
		running_task->length--;
		return running_task;	
	} else if( time >0 && running_task->length == 0){	// remaining time but process is ready
		free(running_task);
		running_task = queue_poll(q);
		if (running_task != NULL){
			time = ts - 1;
			running_task->length--;
		}	
		return running_task;	
	}
	}
	else {					// there is no running process before the tick
		running_task = queue_poll(q);
		if (running_task != NULL){
			time = ts - 1;
			running_task->length--;
		}
		 return running_task;
	}
	return NULL;
}

void finish_task_RR()
{
    // TODO optional
}
