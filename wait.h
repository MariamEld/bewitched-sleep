#ifndef WAIT_H
#define WAIT_H

#include "Util.h"

#define RES_CHAIRS_INDEX(x) ((x % NUMBER_CHAIRS))

int volatile chairs_front;
int volatile chairs_rear;
int volatile chairs_occupied;

sem_t chairs[NUMBER_CHAIRS];

int volatile sids[NUMBER_CHAIRS];
int volatile being_helped;
sem_t students_ready;
sem_t ta_finished_helping;
bool volatile ta_busy;

//Get a waiting chair to sit in
sem_t* reserveChair();

//Get a chair to be served by the barber
sem_t* serveChair();

//Initialize all semaphores
void initialize_semaphores();

//CAUTION: THIS FUNCTION MUST BE GUARDED BY STATE MUTEXES
void writeStatusToFile();
#endif
