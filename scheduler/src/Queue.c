#include "Queue.h"
#include "../include/task.h"

#include <stdio.h>
#include <stdlib.h>

int compare_index(const void *a,const void *b) {
	int a1 = ((def_task *)a)->id; 
    	int b1 = ((def_task *)b)->id;
	
	if (b1 >= a1) return 0;
	else return (a1 - b1);
}

int compare_length(const void *a,const void *b) {
	int a1 = ((def_task *)a)->length; 
    	int b1 = ((def_task *)b)->length;
	
	if (b1 >= a1) return 0;
	else return (a1 - b1);
}

int put_in_end(const void *a,const void *b) {
	
	 return 0;
}

Queue* queue_new(int (*comparator)(const void *a, const void *b))
{
	Queue *queue = (Queue*) malloc(sizeof(Queue));
	if (!queue)
	{
		printf("Could not allocate memory for priority queue.\n");
		exit(1);
	}
	if (comparator == NULL)
	{
		printf("queue_new: No comparator given...\n");
		exit(1);
	}
	else
	{
		queue->comparator = comparator;
	}
	queue->size = 0;
	queue->head = NULL;

	return queue;
}

void queue_free(Queue *queue)
{
	q_elem *current = queue->head;
	q_elem *toDelete;
	while (current != NULL)
	{
		toDelete = current;
		current = current->next;
		free(toDelete->task);
		free(toDelete);
	}
	free(queue);
}

void* queue_push_to_front(Queue *queue, def_task *task) {
    q_elem *newElem = (q_elem*) malloc(sizeof(q_elem));
    if (!newElem)
    {
        printf("Could not allocate memory for queue element.");
        exit(1);
    }

    newElem->task = task;

    if (queue->head == NULL) {
        queue->head = newElem;
        newElem->next = NULL;
    } else {
        newElem->next = queue->head;
        queue->head = newElem;
    }

    queue->size++;
    return task;
}

void* queue_offer(Queue *queue, def_task *task)
{
	q_elem *newElem = (q_elem*) malloc(sizeof(q_elem));
	if (!newElem)
	{
		printf("Could not allocate memory for queue element.");
		exit(1);
	}

	newElem->task = task;

	q_elem *current;
	q_elem *last = NULL;
	for (current = queue->head; current != NULL; current = current->next)
	{
		if (queue->comparator(current->task, newElem->task) > 0)
		{
			break;
		}
		last = current;
	}
	newElem->next = current;
	if (last != NULL) {
	    last->next = newElem;
	} else {
	    queue->head = newElem;
	}

	queue->size++;
	return task;
}

def_task* queue_peek(Queue *queue)
{
	if (queue->size == 0)
	{
		return NULL;
	}
	return queue->head->task;
}

def_task* queue_poll(Queue *queue)
{
	if (queue->size == 0)
	{
		return NULL;
	}

	q_elem *newHead = queue->head->next;
	def_task* task = queue->head->task;
	free(queue->head);
	queue->head = newHead;
	queue->size--;

	return task;
}

int queue_size(Queue *queue)
{
	return queue->size;
}
