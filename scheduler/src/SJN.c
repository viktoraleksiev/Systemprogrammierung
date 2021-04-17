#include "SJN.h"
#include "../include/task.h"
#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *q;

int init_SJN()
{
   q = queue_new(compare_length);
   running_task = NULL;
	if (!q){
		printf("Could not allocate memory for queue.");
		exit(1);
	}
	else return 0;
}

void free_SJN()
{
    free(q);
}

void arrive_SJN(int id, int length)
{	
	if (id >=0 && length > 0){
		def_task * new = malloc(sizeof(def_task));
		new->id = id;
		new->length = length;
		if (running_task != NULL){	
			queue_offer(q,new);
		} 
		else running_task = new;	
	}
}

def_task *tick_SJN()
{
   	if (running_task->length >0){
		running_task->length--;
		return running_task;
	}
	else {
		free(running_task);
		running_task = queue_poll(q);
		if (running_task != NULL) running_task->length--;
		return running_task;
	}
}

void finish_task_SJN()
{
    // TODO optional
}
