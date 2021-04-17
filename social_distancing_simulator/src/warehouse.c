#include "./../include/warehouse.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct warehouse{
    unsigned int flour, sugar, choclate;
    int locked;
    
    unsigned int sugar_taken, flour_taken, choclate_taken;
    //TODO
	pthread_mutex_t mutex1;
	pthread_mutex_t mutex2;	
	pthread_mutex_t mutex3;
	pthread_mutex_t mutex4;
	pthread_mutex_t mutex5;
	pthread_mutex_t mutex6;
	pthread_cond_t cond;
    
};

warehouse* warehouse_create(){
    warehouse* new_warehouse = (warehouse*) malloc(sizeof(warehouse));
    if(new_warehouse== NULL){
        printf("Unable to allocate memory in function %s()\n",__func__);
        return NULL;
    }
    new_warehouse->flour = 0;
    new_warehouse->sugar = 0;
    new_warehouse->choclate = 0;
    new_warehouse->locked = 0;
    
    //TODO
	pthread_mutex_init(&(new_warehouse->mutex1),NULL);
	pthread_mutex_init(&(new_warehouse->mutex2),NULL);
	pthread_mutex_init(&(new_warehouse->mutex3),NULL);
	pthread_mutex_init(&(new_warehouse->mutex4),NULL);
	pthread_mutex_init(&(new_warehouse->mutex5),NULL);
	pthread_mutex_init(&(new_warehouse->mutex6),NULL);
    	pthread_cond_init(&(new_warehouse->cond),NULL);


    new_warehouse->choclate_taken = 0;
    new_warehouse->flour_taken = 0;
    new_warehouse->sugar_taken = 0;
    return new_warehouse;
}

//FUNCTIONS CALLED BY WORKER THREADS
//@param: amount of flour/sugar/choclate to be taken from warehouse wh

unsigned int get_choclate(unsigned int amount, warehouse* wh){
    pthread_mutex_lock(&wh->mutex1);
    wh->choclate_taken += amount;
    pthread_mutex_unlock(&wh->mutex1);

    pthread_mutex_lock(&wh->mutex2);
    wh->choclate -= amount;
    pthread_mutex_unlock(&wh->mutex2);
    
    
    return amount;
}

unsigned int get_flour(unsigned int amount, warehouse* wh){
    
    //TODO
	pthread_mutex_lock(&wh->mutex3);
    wh->flour_taken += amount;
	pthread_mutex_unlock(&wh->mutex3);

	pthread_mutex_lock(&wh->mutex4);
    wh->flour -= amount;

	pthread_mutex_unlock(&wh->mutex4);
    
    return amount;
}

unsigned int get_sugar(unsigned int amount, warehouse* wh){
    
    //TODO
    	pthread_mutex_lock(&wh->mutex5);
    wh->sugar_taken += amount;
    pthread_mutex_unlock(&wh->mutex5);

	pthread_mutex_lock(&wh->mutex6);
    wh->sugar -= amount;
        pthread_mutex_unlock(&wh->mutex6);
    
    return amount;
}


//FUNCTIONS CALLED BY SUPPLIER THREADS
//@param: amount of flour/sugar/choclate to be store in warehouse wh
void deposit_choclate(unsigned int amount, warehouse* wh){
    //TODO
 	pthread_mutex_lock(&wh->mutex2);
    if(wh->choclate + amount > MAX_STORE_CAPACITY){
        wh->choclate = MAX_STORE_CAPACITY;
    	pthread_mutex_unlock(&wh->mutex2);
    }else{
        wh->choclate += amount;
    	pthread_mutex_unlock(&wh->mutex2);
    }

    
    
    return;
}

void deposit_sugar(unsigned int amount, warehouse* wh){
    //TODO
	pthread_mutex_lock(&wh->mutex6);
    if(wh->sugar + amount > MAX_STORE_CAPACITY){
        wh->sugar = MAX_STORE_CAPACITY;
    	pthread_mutex_unlock(&wh->mutex6);
    }else{
        wh->sugar += amount;
    	pthread_mutex_unlock(&wh->mutex6);
    }

    
    return;
}

void deposit_flour(unsigned int amount, warehouse* wh){
    //TODO
    	pthread_mutex_lock(&wh->mutex4);
    if(wh->flour + amount > MAX_STORE_CAPACITY){
        wh->flour = MAX_STORE_CAPACITY;
    	pthread_mutex_unlock(&wh->mutex4);
    }else{
        wh->flour += amount;
    	pthread_mutex_unlock(&wh->mutex4);
    }

    
    return;
}


//FUNCTIONS CALLED BY BAKERY
void lock_warehouse(warehouse* wh){
    //TODO
    wh->locked=1;
	//pthread_cond_broadcast(&wh->cond);

}

unsigned int get_used_resources(warehouse* wh, int type){
    if(type == SUGAR) return wh->sugar_taken;
    if(type == FLOUR) return wh->flour_taken;
    if(type == CHOCLATE) return wh->choclate_taken;
    return 0;
}


void warehouse_destroy(warehouse* wh){
    //TODO
	pthread_mutex_destroy(&wh->mutex1);
	pthread_mutex_destroy(&wh->mutex2);
	pthread_mutex_destroy(&wh->mutex3);
	pthread_mutex_destroy(&wh->mutex4);
	pthread_mutex_destroy(&wh->mutex5);
	pthread_mutex_destroy(&wh->mutex6);
//	pthread_cond_destroy(&wh->cond);
    free(wh);
    return;
}
