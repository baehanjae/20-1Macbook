#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include<sys/ioctl.h>
#include<pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 512

// struct for message buffer
typedef struct {
	char message[512];			// message buffer
	int filled;					// 1 if the message is filled. 0 otherwise.
} Buffer;

void* sender(void* v);
void* receiver(void* v);


int repeat_receiver = 1;

int main(int argc, char* argv[])
{
	
	if (argc == 1) 
	{ 
		printf("Usage: %s <snd_key> <rcv_key>\n", argv[0]);
		return 0;
	}

	printf("snd_key = %s, rcv_key = %s\n",argv[1],argv[2]);

	key_t sndkey_id;
	key_t rcvkey_id;
	int i; 

	sndkey_id = msgget((key_t)atoi(argv[1]),IPC_CREAT|0666);
	if ( sndkey_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}

	
	rcvkey_id = msgget((key_t)atoi(argv[2]),IPC_CREAT|0666);
	if ( rcvkey_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}

	pthread_t p_thread[2];
	int snd_tid;
	int rcv_tid;

	snd_tid = pthread_create(&p_thread[0], NULL, sender,(void*) &sndkey_id);
	rcv_tid = pthread_create(&p_thread[1], NULL, receiver,(void*) &rcvkey_id);

	pthread_join(p_thread[0],NULL);
	pthread_join(p_thread[1],NULL);

	msgctl(sndkey_id,IPC_RMID,0) ;
	msgctl(rcvkey_id,IPC_RMID,0) ;	
	
	return 0;
}

void* sender(void* v)
{
	char buffer[BUFFER_SIZE];
	buffer[0] = '\0';

	while(1)
	{	
		printf("[msg]");
		fgets(buffer,BUFFER_SIZE,stdin);
		buffer[strlen(buffer)-1] = '\0';
		
		if(strcmp(buffer, "quit") == 0)
		{
			repeat_receiver = 0;
			break;
		}

		msgsnd(*(int*)v,buffer,sizeof(buffer),IPC_NOWAIT);
	}
}

void* receiver(void* v)
{
	struct winsize w;
	ioctl(0,TIOCGWINSZ,&w);
	int halfc = (w.ws_col/2) - 5;
	
	char buffer[BUFFER_SIZE];

	while(repeat_receiver)
	{
		buffer[0] = '\0';
		if(msgrcv(*(int*)v,buffer,sizeof(buffer),0,IPC_NOWAIT)==-1) buffer[0] = '\0';
		if (strlen(buffer) != 0)
		{
			int i;
			for(i=0;i<halfc;i++) fprintf(stderr," ");
			fprintf(stderr,"[incoming] \"%s\"\n[msg]",buffer);
		}
		usleep(1000);
	}
}
