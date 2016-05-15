#ifndef WAIT_H
#define WAIT_H

#include "Util.h"

#define RES_CHAIRS_INDEX(x) ((x % NUMBER_CHAIRS))

int chairs_front = 0;
int chairs_rear = -1;
int chairs_occupied = 0;

sem_t chairs[NUMBER_CHAIRS];
sem_t chairs_turn[NUMBER_CHAIRS];

int sids[NUMBER_CHAIRS]
sem_t ta_finished_helping;

//Get a waiting chair to sit in
sem_t* reserveChair();

//Get a chair to be served by the barber
sem_t* serveChair();

//Initialize all semaphores
void initialize_semaphores();

#endif
