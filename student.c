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
    printf("[#%d]: I need some help\n", *_sid);
    LOCK_STATE;                                      //mutex protect the state of the chairs
    sem_t* _sem = reserveChair();
  if ( _sem != NULL ) {
    sem_getvalue(&ta_finished_helping, &_sem_val);
    if(chairs_occupied == 1 && _sem_val == 0){
      printf("[#%d]: Waking up the TA\n", *_sid);
    }
    else{
      printf("[#%d]: Sitting on chair %d\n", *_sid, chairs_rear);
    }
    sem_post(_sem);
    _rear = chairs_rear;
    UNLOCK_STATE;                                     //unlocking mutex that protects the state of the chairs
    sids[chairs_rear] = *_sid;
    sem_wait(&chairs_turn[_rear]);
    printf("[#%d]: TA said he can help me, now entering his room for some time that depends on him\n", *_sid);
    sem_wait(&ta_finished_helping);
    printf("[#%d]: TA finished helping me\n", *_sid);
  }
    else{
      UNLOCK_STATE;                                   //unlocking mutex that protects the state of the chairs
      printf("[#%d]: Tried to get help but couldnot due to chairs capacity, will return to programming\n", *_sid);
    }
  }
}
