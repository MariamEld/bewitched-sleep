/*
 * bewitched-sleep: A C solution to the sleeping TA problem, a version of the sleeping barber problem
 * Written by: Islam Faisal (decltypeme)
 * The American University in Cairo
 * For License, please see LICENSE
 */

/*
 * File: Util.h
 * Author: Islam Faisal
 * Auxiliary declarations and functions
 */
#ifndef UTIL_H
#define UTIL_H

//Some Includes
#include "include.h"

unsigned int seed_r;
int volatile render_row;
#define MUTEXES_COUNT 2
pthread_mutex_t mtxs[MUTEXES_COUNT];

#define MAXIMUM_TIME_ALLOWED 7

FILE *fp;
pthread_rwlock_t rwlock;
//Some Aliases for mutexes
#define RAND_MUTEX mtxs[0]
#define CHAIR_STATE_MUTEX mtxs[1]

#define LOCK_STATE pthread_mutex_lock(&CHAIR_STATE_MUTEX)
#define UNLOCK_STATE pthread_mutex_unlock(&CHAIR_STATE_MUTEX)
#define WRITE_WHAT_IN_BUFFER fwrite(buf, sizeof(buf[0]), strlen(buf), fp)
int generateAPeriodOfTime();

void initializeAllMutexes();

void destroyAllMutexes();

void SIG_INT_HANDLER(int sig);


#endif
