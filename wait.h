



#include "Util.h"
#include <semaphore.h>

#define RES_CHAIRS_INDEX(x) ((x % NUMBER_CHAIRS))

int chairs_front = 0;
int chairs_rear = -1;
int chairs_occupied = 0;

sem_t chairs[NUMBER_CHAIRS];
int sids[NUMBER_CHAIRS]
sem_t ta_so_busy;

//Get a waiting chair to sit in
sem_t* reserveChair();

//Get a chair to be served by the barber
sem_t* serveChair();

//Initialize all semaphores
void initialize_semaphores();
