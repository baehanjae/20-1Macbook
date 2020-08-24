#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
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


int main(int argc, char* argv[])
{
  int duration;
  printf("%d",argc);
  if(argc = 2) duration = atoi(argv[1]);
  else if(argc == 1) duration = 30;


  srand(time(NULL));
  pthread_mutex_init(&mutex,NULL);
  sem_init(&full,0,0);
  sem_init(&empty,0,BUFFER_SIZE);

  DisplayBuffer();

  pthread_t p, c;
  pthread_create(&p,NULL,producer,NULL); //쓰레드 1 생성 (sam_snd 호출)
  pthread_create(&c,NULL,consumer,NULL); //쓰레드 2 생성 (sam_rev 호출)


  int empty_temp;
  while(duration>0){
    Sleep(1000);
    printf("%d ",duration);
    duration--;
    sem_getvalue(&empty,&empty_temp);
    printf("%d\n",empty_temp);
  }

  repeat = 0;

  int temp;
  sem_getvalue(&full,&temp);
  printf("%d\n",temp);
  if(temp == 0){
    sem_post(&full);
  }

  if(empty_temp == -1){
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
  printf("producer\n");
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

    Sleep((rand()%3+1)*1000);

    message = messages[rand()%no_messages];

    printf("[Producer]  Created a message \"%s\"\n",message);

    int temp;
    sem_getvalue(&empty,&temp);
    printf("here is before wait : %d\n",temp);
    sem_wait(&empty);
    sem_getvalue(&empty,&temp);
    printf("here is after wait : %d\n",temp);
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

    sem_getvalue(&full,&in);
    in = in%BUFFER_SIZE;
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
  printf("consumer\n");
  while(repeat){

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
