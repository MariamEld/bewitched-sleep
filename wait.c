#include "wait.h"

sem_t* reserveChair()
{
  if(chairs_occupied == NUMBER_CHAIRS)
  {
    return NULL;
  }
  else
  {
    chairs_rear = (chairs_rear + 1) % NUMBER_CHAIRS;
    chairs_occupied++;
    return &chairs[chairs_rear];
  }
}

sem_t* serveChair()
{
  sem_t* to_return;
  if(chairs_occupied == 0)
  {
    return NULL;
  }
  else
  {
    to_return = &chairs[chairs_front];
    chairs_front = (chairs_front + 1) % NUMBER_CHAIRS;
    chairs_occupied--;
    return to_return;
  }
}

void initialize_semaphores()
{
  sem_init(&students_ready, 0, 0);
  sem_init(&ta_finished_helping, 0, 0);
  int i;
  for(i = 0; i < NUMBER_CHAIRS; i++)
  {
    sem_init(&chairs[i], 0, 0);
  }
}
