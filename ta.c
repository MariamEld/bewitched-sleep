#include "ta.h"

void *ta_loop(void *param)
{
  //Some data Initializations
  int _sid;
  sem_t* _sem;

  while(should_run) {
  //Some sleep if nothing to do
  LOCK_STATE;                                      //mutex protect the state of the chairs
  if(chairs_occupied == 0)                         //Go and look in the waiting room; WE DO NOT CARE IF SOMEONE ARRIVED AFTER WE LOOK. THEY SHOULD NOTIFY US.
    printf("[TA]: I am sleeping, signal me to wake me\n");
  UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
  sem_wait(&chairs[chairs_front]);                 //wait for the semaphore at th front of the queue to be signaled by a student
  LOCK_STATE;                                      //mutex protect the state of the chairs
  _sid = sids[front];
  _sem = serveChair();
  sem_post(&_sem);                                 //Come for help, dear student
  UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
  int _period = generateAPeriodOfTime();
  printf("[TA]: I am helping student #%d for time %d, the chairs are outside!", _sid, _period);
  sleep(_period);
  }
  printf("[TA]: I am out of service, simulation is done!\n");
}
