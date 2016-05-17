# bewitched-sleep
A C solution to the sleeping TA problem, a version of the sleeping barber problem

## System Requirments

Unless no SIG_INT is signaled:
1. If there are no students, the TA takes a nap.
2. If a student comes while the TA is sleeping, they can signal him to awaken him.
3. If different students are waiting, they are helped on a FIFO basis.
4. If all waiting chairs are occupied, the student gets back to program.
5. Programming and helping times are generated by a pseudo-random number generator.
6. A waiting student will be helped sometime in the future.
7. A student is either programming, waiting or being helped.
8. The TA is either helping or sleeping (or checking, but not important though).

And
9. The simulation must halt peacefully. All waiting students should be helped and anyone programming, helping or being helped for a period of time should complete this period of time doing the corresponding activity.

## How it works

Assumption: We assume that a first person arriving will sit on a chair and then immediately will be invited for help.

### Variables

We use a mix of global mutexes, flags, locks and semaphores to satisfy the previously mentioned requirments.

#### Volatile Flags/Variables

**Any changes to these must be guarded by the system state mutex.**

1. ```should_run```: A flag that is true as long as no SIG_INT received.
2. ```really_stop_nothing_to_do```: A flag that is raised when all students travel for the summer vacation. **This ensures that the TA does not keep sleeping (some sort of bounded waiting)**.
3. ```ta_busy```: True if and only if the TA is helping someone.
4. ```being_helped```: a positive student id if ```ta_busy``` and 0 otherwise.

#### Mutexes

1. ```RAND_MUTEX```: To avoid two threads accessing ```rand_r()``` at the same time.
2. ```CHAIR_STATE_MUTEX```: To avoid two entities changing the system state at the same time and avoid confusion while reading the system state.

#### Semaphores

All semaphores are initialized to a value of 1 to make sure wait is not encountered until a signal is issued.

1. ```Chairs```: We associate a semaphore with each chair. This association ensures that we can control the semaphore scheduling in a FIFO manner. Leaving it to the operating system does not guarantee a FIFO policy. When a **student** sits on a chair, they **wait** on the chair's corresponding semaphore. 
2. ```students_ready```: A *student* *signals* this to the TA to let them know they need help if they either wait or arrive and they are sleeping. The TA sleeping means he is waiting on this signal. This signal is issued when SIG_INT is issued, but in a special manner by setting a volatile flag called ```really_stop_nothing_to_do```.
3. ```ta_finished_helping```: is *signaled* when the *TA* is done because he determines the period of time. This must be *awaited* by and only be the *student being helped*.

#### Read/Write Locks
We use read/write locks to safely write to the SVG file that is shared by all the threads. Please see below for details.

### Halting

When halting, the flag ```should_run``` is set to false. The flag ```really_stop_nothing_to_do``` is set to true, but the latter setting seriously needs a mutex because it is a change of the state of the system.

## Verification of Requirments

We verify in the same order of listing above. Too lazy to type again.

1. The TA waits on ```students_ready``` means the TA is sleeping. If there are no students (nor SIG_INT), or TA is having sweet dreams because those are the one and only ones who can signal this semaphore.
2. Again, the TA waits on ```students_ready```, each new student waiting or wanting to enter the room and have taken serious steps (reserved a chair) must signal a ```students_ready``` which is enough to awake the TA.
3. This is guaranteed by the queue structure provided. The TA resolves the next semaphore to help from this data structure.
4. This is guaranteed by the nature of the ```reserve_chair()``` function which will check on the state in a guarded manner.
5. Thread-safety of the pseudo-random number generator is discussed below.
6. The students before him will be helped for a finite time and then they will be signaled that their help is over and therefore continue life. Even when SIG_INT is trigerred, we let the awaiting students be helped.
7. Follows from the loop structure and requirment 6.
8. By the nature of the loop and the bounded waiting of the TA in helping and that sleeping is interruptible by students or SIG_INT

9. This is done using the flags mentioned above.
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
#### Examples of behaviors that must be guarded due to changing/reading the system state include:
1. (En/De)queing from/into the waiting queue
2. Writing the waiting/helping information to files or to the screen
3. Changing shared volatile flags/variables.

**Each thread must ensure that they do not lock the mutex and go to sleep or wait for signals!!! Threads, please be considerate.**


### File-Writing
We use a lock to control writing to the file across different threads.

```C
pthread_rwlock_wrlock(&rwlock);				//Acquire the lock on the file
.....
WRITE_WHAT_IN_BUFFER;
pthread_rwlock_unlock(&rwlock);				//Release the lock on the file
```
**The function ```writeStatusToFile()``` must be called while a mutex is locked. Writing to the file also requires locking and unlocking the file lock.**

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
It is important to end the simulation safely to close file buffers, end threads safely and most importantly to let the students and TAs finish their jobs and not to interrupt programming, waiting, helping or sleeping(just kidding, a **SIG_INT would awake the TA, so that he can catch his flight to Paris.**)

## SVG Rendering
If you choose to have 3 waiting chairs, we can reward you with an elegant SVG rendering feature. After you halt the simulation, you just open the SVG file that will be created in the same folder. The file can have a relatively large height, so you may want to zoom in.

**Bonus Exercise: How many 42s are there in the SVG file?**

### Examples of the rendering:

1. Red: Means not part of the queue.
2. Yellow: Front and all in the queue
3. Blue: Rear Element. In case of one element, the rear is the front and is colored yellow.
4. Grey: TA Sleeping
5. Green: TA Serving someone

<img src="https://raw.githubusercontent.com/decltypeme/bewitched-sleep/master/examples/1.svg">


<img src="https://raw.githubusercontent.com/decltypeme/bewitched-sleep/master/examples/2.svg">
