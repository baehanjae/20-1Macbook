#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BUFFER_SIZE 5
#define MAX_MESSAGE 64

void Error(const char *mes);

//mutex와 semaphore 선언
pthread_mutex_t mutex;
sem_t empty, full;

//초반코딩의 패해
sem_t bin_sem;
int number;

//공유자원인 global variable 설정
char buffer[BUFFER_SIZE][MAX_MESSAGE];
int in = 0, out = 0, count = 0;

//프로그램 종료를 위한 global variable
int repeat = 1;

//display 함수 선언
void DisplayBuffer();

//thread 실행부분
void *sem_snd(void *arg);
void *sem_rev(void *arg);
void *producer(void *arg);
void *consumer(void *arg);

//thread parameter
typedef struct _t_arg{
  int* empty_temp_ptr;
  int* full_temp_ptr;
} t_arg;


int main(int argc, char* argv[])
{
  int duration;
  if(argc = 2) duration = atoi(argv[1]);
  else if(argc == 1) duration = 30;


  srand((unsigned int)time(NULL));
  pthread_mutex_init(&mutex,NULL);
  sem_init(&full,0,0);
  sem_init(&empty,0,BUFFER_SIZE);

  DisplayBuffer();

  pthread_t p, c;
  t_arg* t_argment;
  t_argment = (t_arg*)malloc(sizeof(t_argment));
  int empty_temp;
  int full_temp;
  t_argment->empty_temp_ptr = &empty_temp;
  t_argment->full_temp_ptr = &full_temp;
  pthread_create(&p,NULL,producer,t_argment); //쓰레드 1 생성 (sam_snd 호출)
  pthread_create(&c,NULL,consumer,t_argment); //쓰레드 2 생성 (sam_rev 호출)


  while(duration>0){
    sleep(1);
    duration--;
  }

  repeat = 0;


  if(full_temp == 0){
    sem_post(&full);
  }

    int temp;
    sem_getvalue(&empty,&temp);
    printf("after repaet 0 empty = %d\n",temp);
    if(temp == 0){
    sem_post(&empty);
  }


  pthread_join(p,NULL); //쓰레드가 종료되기를 기다린 후 메인 프로세스가 종료되도록
  pthread_join(c,NULL); //pthread_join 을 호출

  pthread_mutex_destroy(&mutex);
  sem_destroy(&full);
  sem_destroy(&empty);

  printf("Bye!\n");
  return 0;
}


void *producer(void *arg){
  t_arg* myarg = (t_arg*)arg;
  int no_messages = 10;
  char *messages[MAX_MESSAGE] = {
    "Nice to see you!",
    "Aal izz well!",
    "I love you! God loves you!",
    "God loves you and has a wonderful plan for your life.",
    "God bless you!",
    "You are genius!",
    "Cheer up!",
    "Everything is gonna be okay.",
    "You are so precious",
    "Hakuna matata!"
  };
  char* message;
  while(repeat){

    sleep((rand()%3+1));

    message = messages[rand()%no_messages];

    printf("[Producer]  Created a message \"%s\"\n",message);

    sem_getvalue(&empty,myarg->empty_temp_ptr);
    //printf("here is before wait : %d\n",myarg->empty_temp_ptr[0]);
    sem_trywait(&empty);
    pthread_mutex_lock(&mutex);

    for(int i=0;i<22;i++){
      printf("-");
    }
    printf(" PRODUCER ");
    for(int i=0;i<22;i++){
      printf("-");
    }
    printf("\n");
    printf("Producer is entering critical section.\n");


    for(int i =0;i<strlen(message);i++){
      buffer[in][i] = *(message + i);
    }
    in = (in+1)%BUFFER_SIZE;

    printf("[Producer] \"%s\" ==> buffer\n",message);
    count++;

    DisplayBuffer();

    printf("Producer is leaving critical section.\n");
    for(int i=0;i<54;i++){
      printf("-");
    }
    printf("\n");

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
}
void *consumer(void *arg){
  sleep(4);
  t_arg* myarg = (t_arg*)arg;
  char* message;
  while(repeat){
    sem_trywait(&full);
    pthread_mutex_lock(&mutex);

    for(int i=0;i<22;i++){
      printf("-");
    }
    printf(" CONSUMER ");
    for(int i=0;i<22;i++){
      printf("-");
    }
    printf("\n");
    printf("Consumer is entering critical section.\n");

    message = buffer[out];
    count--;
    out = (out+1)%BUFFER_SIZE;

    printf("[Consumer] buffer ==> \"%s\"\n",message);

    DisplayBuffer();

    printf("Consumer is leaving critical section.\n");
    for(int i=0;i<54;i++){
      printf("-");
    }
    printf("\n");

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    printf("[Consumer]  Consumed a message \"%s\"\n",message);

    sleep((rand()%3+2));
  }
}

void DisplayBuffer(){
  printf("Buffer contents:\n");
  printf("\tcount = %d, in = %d, out = %d \n",count, in, out);
  int p = out;
  for(int i = 0; i < count; i++){
    printf("\t%d ] %s \n", p, buffer[p]);
    p = (p + 1) % BUFFER_SIZE;
  }
}
