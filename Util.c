/*
 * bewitched-sleep: A C solution to the sleeping TA problem, a version of the sleeping barber problem
 * Written by: Islam Faisal (decltypeme)
 * The American University in Cairo
 * For License, please see LICENSE
 */

/*
 * File: Util.c
 * Author: Islam Faisal
 * Auxiliary declarations and functions
 */
#include "Util.h"

void actOnMutexes(bool initialize_option)
{
  int i, _status;
  for(i = 0; i < MUTEXES_COUNT; i++)
  {
    if(initialize_option)
    {
      if((_status = pthread_mutex_init(&mtxs[i], NULL) != 0))
      {
        fprintf(stderr, "Cannot initailize mutex at index %d and error code is %d\n", i, _status);
      }
    }
    else
    {
      if((_status = pthread_mutex_destroy(&mtxs[i])) != 0)
      {
        fprintf(stderr, "Cannot initailize mutex at index %d and error code is %d\n", i, _status);
      }
    }
  }
}

 void initializeAllMutexes()
 {
   actOnMutexes(true);
 }

 void destroyAllMutexes()
 {
   actOnMutexes(false);
 }

 int generateAPeriodOfTime()
 {
   int generated_rand;
   pthread_mutex_lock(&RAND_MUTEX);
   generated_rand = rand_r(&seed_r);
   pthread_mutex_unlock(&RAND_MUTEX);
   return ( generated_rand % MAXIMUM_TIME_ALLOWED + 1);
 }


 void SIG_INT_HANDLER(int sig) {
   should_run = false;
}
