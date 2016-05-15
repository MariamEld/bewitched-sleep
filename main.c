

#include "wait.h"
#include "student.h"
#include "ta.h"

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


int main(int argc, char** argv)
{
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

  pthread_join(ta_, NULL);
  //Destroy ALl mutexes
  destroyAllMutexes();
  //Free memory
  free(students_);
  free(student_ids);
  exit(0);
}
