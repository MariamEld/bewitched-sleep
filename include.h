#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#define NUMBER_CHAIRS 3


bool volatile should_run;
bool volatile really_stop_nothing_to_do;

#endif
