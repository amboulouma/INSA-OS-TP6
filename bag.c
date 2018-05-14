#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "bag.h"

bag_t * bb_create(int size)
{
    assert(size > 0);
    
    bag_t *bag=malloc(sizeof(bag_t));
    assert(bag != NULL);

    bag->elem = malloc( size * sizeof(void*));
    assert(bag->elem);

    bag->size  = size;
    bag->count = 0;
    bag->is_closed = 0;
    
    //printf("%d\n",bag->semaphore);
    
    
    assert(sem_init(&(bag->semaphore_concurrent), 0, 1)==0);
    assert(sem_init(&(bag->semaphore_limite), 0, bag->count)==0);
    assert(sem_init(&(bag->semaphore_limite), 0, bag->size)==0);

    return bag;
}

void bb_add(bag_t * bag, void * element)
{
    assert(bag != NULL);                 // sanity check

    //while( bag->count >= bag->size ) { } // CAUTION: this synchronization is bogus
    
    assert( bag-> is_closed == 0 );   // adding to a closed bag is an error
    
	sem_wait(&(bag->semaphore_concurrent));
	sem_post(&(bag->semaphore_limite));
	sem_wait(&(bag->semaphore_limite2));
		assert( bag->count < bag->size ); // sanity check
		bag->elem[bag->count] = element;
		bag->count += 1;
    sem_post(&(bag->semaphore_concurrent));
}

void * bb_take(bag_t *bag)
{
    assert(bag != NULL); // sanity check

    //while( bag->count <= 0 ) { } // CAUTION: this synchronization is bogus
    sem_wait(&(bag->semaphore_concurrent));
    sem_wait(&(bag->semaphore_limite));
    sem_post(&(bag->semaphore_limite2));
    
		assert( bag->count > 0); // sanity check
		bag->count -= 1;
		void *r = bag->elem[bag->count];
    sem_post(&(bag->semaphore_concurrent));

    usleep(10);// artificial delay to increase the occurence of race conditions
    return r;
}

void bb_close(bag_t *bag, int N)
{
	sem_destroy(&(bag->semaphore_limite));
	sem_destroy(&(bag->semaphore_limite2));
	sem_destroy(&(bag->semaphore_concurrent));
    assert("not implemented" == 0);
}
