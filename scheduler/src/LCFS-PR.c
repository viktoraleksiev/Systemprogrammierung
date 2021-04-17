#include "LCFS-PR.h"
#include "../include/task.h"
#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue *q;

int init_LCFS_PR()
{
   q = queue_new(compare_index);	 // higher index is put in the bottom
   running_task = NULL;
	if (!q){
		printf("Could not allocate memory for queue.");
		exit(1);
	}
	else return 0;
}

void free_LCFS_PR()
{
    free(q);
}

void arrive_LCFS_PR(int id, int length)
{
	if (id >=0 && length > 0){
		def_task * new = malloc(sizeof(def_task));
		new->id = id;
		new->length = length;
		if (running_task != NULL){
			def_task * old = switch_task(new);
			queue_offer(q,old);
		} 
		else running_task = new;
	}
	
}

def_task *tick_LCFS_PR()
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

void finish_task_LCFS_PR()
{
    // todo optional
}
