#include "ta.h"

void *ta_loop(void *param)
{
  //Some data Initializations
  int _sid;
  sem_t* _sem;

  while(true) {
  //Some sleep if nothing to do
  LOCK_STATE;                                      //mutex protect the state of the chairs
  if(!(should_run || chairs_occupied != 0))
  {
        UNLOCK_STATE;
        break;
  }
  if(chairs_occupied == 0){                         //Go and look in the waiting room; WE DO NOT CARE IF SOMEONE ARRIVED AFTER WE LOOK. THEY SHOULD NOTIFY US.
    printf("[TA]: I am sleeping, signal me to wake me!\n");
  }
  UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
  sem_wait(&students_ready);                 //wait for the semaphore at th front of the queue to be signaled by a student
  LOCK_STATE;                                   //mutex protect the state of the chairs
  if(really_stop_nothing_to_do)
  {
    UNLOCK_STATE;
    break;
  }                 
  _sid = sids[chairs_front];
  _sem = serveChair();
  sem_post(_sem);                   //Come for help, dear student
  ta_busy = true;
  UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
  int _period = generateAPeriodOfTime();
  printf("[TA]: I am helping student #%d for time %d!\n", _sid, _period);
  sleep(_period);
  LOCK_STATE;
  printf("[TA]: I finished helping student #%d!\n", _sid);
  sem_post(&ta_finished_helping);
  ta_busy = false;
  UNLOCK_STATE;
  }
  printf("[TA]: I am out of service!\n");
}
