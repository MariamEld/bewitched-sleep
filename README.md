# bewitched-sleep
A C solution to the sleeping TA problem, a version of the sleeping barber problem

## How it works
### Conditions

### Variables



## Build, Start and Terminate

To build, you either invoke build.sh or execute the following
```
gcc *.c -pthread -o sleeping-ta
chmod +x sleeping-ta
```
To invoke the sleeping TA, you simply call the binary and pass the number of students
```
./sleeping-ta
```
To terminate the simulation safely, you pass a SIG_INT to the process. You can simply do so on your terminal if it is running in the foreground by simply pressing Ctrl + C.
It is important to end the simulation safely to close file buffers, end threads safely and most important to let the students and TAs finish their jobs and not to interrupt programming, waiting, helping or sleeping(just kidding, a SIG_INT would awake the TA, so that he can catch his flight to Paris.)
## SVG Rendering
If you choose to have 3 waiting chairs, we can reward you with an elegant SVG rendering feature. After you halt the simulation, you just open the SVG file that will be created in the same folder. The file can have a relatively large height, so you may want to zoom in. Bonus Exercise: How many 42s are there in the SVG file?

## Thread Safety Concerns

### Pseudo-random number generation
We use a mutex to guard the usage of the pseudo-random number generator and use the function rand_r which is thread safe.
```C
   pthread_mutex_lock(&RAND_MUTEX);
   generated_rand = rand_r(&seed_r);
   pthread_mutex_unlock(&RAND_MUTEX);
```

### System-state
Changing the system state can be done in many ways. To make sure that two students cannot grab the same chair or ensure that other weird things will not happen, we use a mutex to guard critical sections that change the state of the system.
```C
LOCK_STATE; 				//#define LOCK_STATE pthread_mutex_lock(&CHAIR_STATE_MUTEX)
//Change System State
UNLOCK_STATE;				//#define UNLOCK_STATE pthread_mutex_unlock(&CHAIR_STATE_MUTEX)
```
#### Examples of behaviors that must be guarded due to changing/reading the system state include
1. (En/De)queing from/into the waiting queue
2. Writing the waiting/helping information to files or to the screen
3. 

**Each thread must ensure that they do not lock the mutex and go to sleep or wait for signals!!! Threads, please be considerate.**


### File-Writing
We use a lock to control writing to the file across different threads.

```C
pthread_rwlock_wrlock(&rwlock);				//Acquire the lock on the file
.....
WRITE_WHAT_IN_BUFFER;
pthread_rwlock_unlock(&rwlock);				//Release the lock on the file
```
**The function ```writeStatusToFile()``` must be called while a mutex is locked. Writing to the file also requires locking and unlocking the file lock**
