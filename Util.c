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
   if(pthread_rwlock_init(&rwlock, NULL) != 0)
	fprintf(stderr, "Couldnot initialize the rwlock\n");
 }

 void destroyAllMutexes()
 {
   actOnMutexes(false);
   if(pthread_rwlock_destroy(&rwlock) != 0)
	fprintf(stderr, "Couldnot destroy the rwlock\n");
 }

 int generateAPeriodOfTime()
 {
   int generated_rand;
   pthread_mutex_lock(&RAND_MUTEX);
   generated_rand = rand_r(&seed_r);
   pthread_mutex_unlock(&RAND_MUTEX);
   return ( generated_rand % MAXIMUM_TIME_ALLOWED + 1);
 }

//LOCKING HERE MAY CAUSE SERIOUS PROBLEMS BECAUSE YOU MAY BE IN A LOCK ALREADY
//YOU ARE THE ONLY ONE CHANGING
 void SIG_INT_HANDLER(int sig) {
   printf("\tOkay, we are wrapping up the simulation\n");
   should_run = false;
}


