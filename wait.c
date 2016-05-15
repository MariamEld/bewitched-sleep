sem_t* reserveChair()
{
  if(chairs_occupied == NUMBER_CHAIRS)
  {
    return NULL;
  }
  else
  {
    chairs_rear = RES_CHAIRS_INDEX(chairs_rear + 1);
    chairs_occupied++;
    return chairs[chairs_rear];
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
    to_return = chairs[chairs_front];
    chairs_front = RES_CHAIRS_INDEX(chairs_front + 1);
    chairs_occupied--;
    return to_return;
  }
}
