#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
sem_t mutex,writeblock;
int data = 0,rcount = 0;

void *reader(void *arg)
{
  int *f;
  f = ((int *)arg);
  sem_wait(&mutex);
  rcount = rcount + 1;
  if(rcount==1)
   sem_wait(&writeblock);
  sem_post(&mutex);
  printf("Data read by the reader%d is %d\n",*f,data);
  sleep(1);
  sem_wait(&mutex);
  rcount = rcount - 1;
  if(rcount==0)
   sem_post(&writeblock);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int *f;
  f = ((int *)arg);
  sem_wait(&writeblock);
  data++;
  printf("Data writen by the writer%d is %d\n",*f,data);
  sleep(1);
  sem_post(&writeblock);
}

int main()
{
  int i,b,*z; 
  pthread_t rtid[5],wtid[5];
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);
  for(i=0;;i++)
  {	z=&i;
    pthread_create(&wtid[i],NULL,writer,(void*)z);
    pthread_create(&rtid[i],NULL,reader,(void*)z);
	pthread_join(wtid[i],NULL);
    pthread_join(rtid[i],NULL);  
}
  
  return 0;
}
