#include "wait.h"

sem_t* reserveChair()
{
  if(chairs_occupied == NUMBER_CHAIRS)
  {
    return NULL;
  }
  else
  {
    chairs_rear = (chairs_rear + 1) % NUMBER_CHAIRS;
    chairs_occupied++;
    //writeStatusToFile();				//Do not call here
    return &chairs[chairs_rear];
  }
}

sem_t* serveChair()
{
  sem_t* to_return;
  if(chairs_occupied == 0)
  {
    return NULL;
  }
  else
  {
    to_return = &chairs[chairs_front];
    chairs_front = (chairs_front + 1) % NUMBER_CHAIRS;
    chairs_occupied--;
    //writeStatusToFile();				//Do not call from here
    return to_return;
  }
}

void initialize_semaphores()
{
  sem_init(&students_ready, 0, 0);
  sem_init(&ta_finished_helping, 0, 0);
  int i;
  for(i = 0; i < NUMBER_CHAIRS; i++)
  {
    sem_init(&chairs[i], 0, 0);
  }
}



//CAUTION: THIS FUNCTION MUST BE GUARDED BY STATE MUTEXES
void writeStatusToFile()
{
  //Acquire the lock on the file
  pthread_rwlock_wrlock(&rwlock);
  render_row++;
  int being_served = 0;
  char buf[2000];
  char color1[10] = "yellow";
  char color2[10] = "red";
  char color3[10] = "grey";
  char color4[10] = "green";
  char color5[10] = "blue";
  char* color = color3;
  if(ta_busy) 
   {
	being_served = being_helped;
	color = color4;
	//It must be that one whom he is busy with
	/*if(chairs_front-1 < 0)
		being_served = sids[chairs_front - 1 + NUMBER_CHAIRS];
	else	
		being_served = sids[(chairs_front - 1)%NUMBER_CHAIRS];*/
   }
   //sprintf(buf, "[%d]Helping %d\n", render_row, being_served);
	
	//The Being Helped Circle
	sprintf(buf, "\t<g><circle cx=\"50\" cy=\"%d\" r=\"42\" stroke=\"black\" stroke-width=\"3\" fill=\"%s\" /><text x=\"50\" y=\"%d\" font-size=\"42\" text-anchor=\"middle\" >%d</text></g>", (100 * render_row + 50), color, (100 * render_row + 50), being_served);
	WRITE_WHAT_IN_BUFFER;	
	int i;
	for(i = 0; i < NUMBER_CHAIRS; i++){
	//The queue Circles
	//choose the color
	if(chairs_occupied == 0)
		color = color2;
	else if(i == chairs_front)
		color = color1;
	else if(i == chairs_rear)
		color = color5;
	else if(chairs_occupied == NUMBER_CHAIRS)
		color = color1;
	else
		color = color2;
	//Now print it
		sprintf(buf, "\t<g><circle cx=\"%d\" cy=\"%d\" r=\"42\" stroke=\"black\" stroke-width=\"3\" fill=\"%s\" /><text x=\"%d\" y=\"%d\" font-size=\"42\" text-anchor=\"middle\" >%d</text></g>", 100*(i+1) + 50, 100 * render_row + 50, color, 100*(i+1) + 50, 100 * render_row + 50, sids[i]);
	WRITE_WHAT_IN_BUFFER;
}
  //Release the lock on the file
  pthread_rwlock_unlock(&rwlock);
}
