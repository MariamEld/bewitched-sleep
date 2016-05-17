#include "student.h"

void *student_loop(void *param)
{
  int* _sid = (int*) param;
  int _rear;
  int _sem_val;
  while(should_run) {
    //Program for some time
    int _period = generateAPeriodOfTime();
    printf("[#%d]: Programming for %d\n", *_sid, _period);
    sleep(_period);
    LOCK_STATE;                                      //mutex protect the state of the chairs
    if(!should_run)
    {
      UNLOCK_STATE;
      break;
    }
    printf("[#%d]: I need some help!\n", *_sid);
    sem_t* _sem = reserveChair();
  if ( _sem != NULL ) {
    if(chairs_occupied == 1 && !ta_busy){
      printf("[#%d]: Waking up the TA!\n", *_sid);
    }
    else{
      printf("[#%d]: Sitting on chair %d\n", *_sid, chairs_rear + 1);
    }
    sem_post(&students_ready);
    sids[chairs_rear] = *_sid;
    UNLOCK_STATE;                                     //unlocking mutex that protects the state of the chairs
    sem_wait(_sem);
    printf("[#%d]: TA said he can help me!\n", *_sid);
    sem_wait(&ta_finished_helping);
    printf("[#%d]: TA finished helping me!\n", *_sid);
  }
    else{
      UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
      printf("[#%d]: Tried to get a chair but couldnot, will return to programming\n", *_sid);
    }
  }
  printf("[#%d]: I withdraw from the infinite semester!\n", *_sid);
}
