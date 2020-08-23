#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE 256
#define ARRAY_SIZE 100

//prioriy queue에 들어가 구조체 선언
typedef struct Data {
    char name[ARRAY_SIZE];
    int key;
}Data;

//heap과 size 선언
Data heap[HEAP_SIZE];
int heap_count = 0;

//swap 함수
void swap(Data * a, Data * b) {
	Data tmp = *a; *a = *b; *b = tmp;
}

//insert에 사용할 push 함수
void push(Data data) {
	heap[++heap_count] = data;

	int child = heap_count;
	int parent = child / 2;
	while (child > 1 && heap[parent].key < heap[child].key) {
		swap(&heap[parent], &heap[child]);
		child = parent;
		parent = child / 2;
	}
}

//delete에 사용할 pop 함수
void pop() {
	swap(&heap[1], &heap[heap_count]);
	heap_count--;

	int parent = 1;
	int child = parent * 2;

	if (child + 1 <= heap_count) {
		child = (heap[child].key > heap[child + 1].key) ? child : child + 1;
	}

	while (child <= heap_count && heap[parent].key < heap[child].key) {
		swap(&heap[parent], &heap[child]);

		parent = child;
		child = child * 2;
		if (child + 1 <= heap_count) {
			child = (heap[child].key > heap[child + 1].key) ? child : child + 1;
		}
	}
}


//'K' key 에 필요한 heapify 와 build_heap 함수
void heapify(Data* heap, int here, int size){
  int left = here * 2;
  int right = here * 2 + 1;
  int max = here;
  if (left<size && heap[left].key > heap[max].key) max = left;
  if (right<size && heap[right].key > heap[max].key) max = right;

  if (max != here) {
    swap(&heap[here], &heap[max]);
    heapify(heap,max,size);
  }
}

void build_heap(Data *heap, int size){
  int i;
  for (i = size / 2; i>0 ; i--){
    heapify(heap,i,size);
  }
}

//메뉴를 출력해주는 함수
void printMenu() {
    printf("********************MENU********************\n");
    printf("I : Insert new element into queue\n");
    printf("D : Delete element with largest key from queue\n");
    printf("R : Retrieve element with largest key from queue\n");
    printf("K : Increase key of element in queue\n");
    printf("P : Print out all elements in queue\n");
    printf("Q : Quit\n\n");

     printf("Choose menu: ");
}

//입력버퍼 처리해주는 함수 : scanf 입력 후 생기는 입력버퍼의 값들을 제거해준다.
void clearReadBuffer() {
 if (stdin->_cnt)
  while (getchar() != '\n');
}

//사용자로 부터 key 값을 받아오는 함수
char input() {
	char key;
	scanf("%c", &key );
	clearReadBuffer();
	return key;
}

// main
int main() {
	while(1) //'Q' 이전까지 반복적인 동작을 실행
	{
		printMenu();
		char key = input();

		if(key == 'Q'){ // key 값이 'Q' 인 경우
			printf("Thank you, Bye!\n\n");
			break;
		}

		else if(key == 'I'){ // key 값이 'I' 인 경우
      char name[100];
    	int priority = -1;
			printf("Enter name of element : ");
			scanf("%s",name);
			clearReadBuffer();
			printf("Enter key value of element : ");
			scanf("%d",&priority);
			clearReadBuffer();
      if (0 >= priority || priority > 10){
        printf("unexpected value.please choose menu again. \n\n\n");
        continue;
      }
			Data data;
			data.key = priority;
			strcpy(data.name,name);
			push(data);
			printf("New elelnent [ %s, %d ] is inserted.\n",name,priority);
		}

		else if(key == 'P'){ // key 값이 'P' 인 경우
      if (heap_count == 0){
        printf("heap is empty!\n\n\n");
        continue;
      }
			int i;
			for (i=1;i<=heap_count;i++){
				printf("[ %s, %d ]  ", heap[i].name,heap[i].key);
			}
			printf("\n");
		}

		else if(key == 'D'){ // key 값이 'D' 인 경우
      if (heap_count == 0){
        printf("heap is already empty!\n\n\n");
        continue;
      }
			printf("[ %s, %d ] is deleted.\n\n\n", heap[1].name,heap[1].key);
			pop();
		}

		else if(key == 'R'){ // key 값이 'R' 인 경우
      if (heap_count == 0){
        printf("heap is empty!\n\n\n");
        continue;
      }
			printf("[ %s, %d ]  ", heap[1].name,heap[1].key);
		}

		else if(key == 'K'){ // key 값이 'K' 인 경우
      if (heap_count == 0){
        printf("heap is empty!\n\n\n");
        continue;
      }
			int index = -1;
			int new_value = -1;
			printf("Enter index of element : ");
			scanf("%d",&index);
			clearReadBuffer();
      if (index == -1 || index < 0 || heap_count < index){
        printf("unexpected index.please choose menu again. \n\n\n");
        continue;
      }
			printf("Enter new key value : ");
			scanf("%d",&new_value);
			clearReadBuffer();
      if (0 >= new_value || new_value > 10){
        printf("unexpected value.please choose menu again. \n\n\n");
        continue;
      }
			heap[index].key = new_value;
  		build_heap(heap,heap_count);
		}

		else{ // key 값 예외 처리
			printf("unexpected input.please choose menu again. \n\n\n ");
			continue;
		}
	}
	return 0;
}
