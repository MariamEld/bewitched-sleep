

#include "student.h"

int* student_ids;
pthread_t ta_;
pthread_t* students_;


void allocate(int _count)
{
  students_ = (pthread_t*) malloc(sizeof(pthread_t) * _count);
  student_ids = (int*) malloc(sizeof(int) * _count);
  int i;
  for (i = 0; i < _count; i++)
  	student_ids[i] = i + 1;
}

void init_()
{
  LOCK_STATE;
  render_row = -1;
  chairs_front = 0;
  chairs_rear = -1;
  chairs_occupied = 0;
  being_helped = 0;
  should_run = true;
  seed_r = 42;
  ta_busy = false;
  really_stop_nothing_to_do = false;
  UNLOCK_STATE;
}

int main(int argc, char** argv)
{
  char opening_tag[10] = "<svg>";
  char closing_tag[10] = "</svg>";

  if(argc != 2)
  {
    fprintf(stderr, "Invalid Parameters ... Exiting ...\n");
    exit(1);
  }
  init_();
  int i;
  //The interrupt signal
  signal(SIGINT, SIG_INT_HANDLER);
  //Initialize All mutexes and semaphores
  initializeAllMutexes();
  initialize_semaphores();
  long int students_count = strtol (argv[1], NULL, 10);
  if(students_count <= 0)
  {
    fprintf(stderr, "Invalid Student Count ... Exiting ...\n");
    exit(1);
  }
  //Open file for writing
  fp=fopen("sleeping-ta.svg", "w+");

  //Though single thread but in case
  pthread_rwlock_wrlock(&rwlock);
  fwrite(opening_tag, sizeof(opening_tag[0]), strlen(opening_tag), fp);
  pthread_rwlock_unlock(&rwlock);

  //Allocate and Initilaize Some things
  allocate(students_count);
  //Create TA ta_thread
  pthread_create(&ta_, 0, ta_loop, 0);
  //Create students threads
  for (i = 0; i < students_count; i++) {
  pthread_create(&students_[i], 0, student_loop, (void *)&student_ids[i]);
  }
  //Join threads
  for (i = 0; i < students_count; i++)
    pthread_join(students_[i], NULL);

LOCK_STATE;
sem_post(&students_ready);
really_stop_nothing_to_do = true;
UNLOCK_STATE;

  pthread_join(ta_, NULL);
  //Destroy ALl mutexes
  destroyAllMutexes();
  //Free memory
  free(students_);
  free(student_ids);
  //Though single thread but in case
  pthread_rwlock_wrlock(&rwlock);
  fwrite(closing_tag, sizeof(closing_tag[0]), strlen(closing_tag), fp);
  pthread_rwlock_unlock(&rwlock);
  //Close
  fclose(fp);
  exit(0);
}
