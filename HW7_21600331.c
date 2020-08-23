#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define MAX_ELEMENT 100

void remove_space(char* s);
int number_city(char* s);
void enter_space(char* s,char* n);

typedef struct __list {
    struct __node *cur;
    struct __node *head;
    struct __node *tail;
} linkedList;
 
typedef struct __node {
    int w;
    int index;
    struct __node *next;
} item;

typedef struct{
    int key;
    int index; //0~9
} node;

typedef struct{
  node* heap[MAX_ELEMENT];
  int heap_size;
} Heap;

void insert_min_heap(Heap *h, node* item);
node* delete_min_heap(Heap *h);

void swap(node** a,node** b){
    node* temp = *a;
    *a = *b;
    *b = temp;
    
}

void heapify(Heap *h, int here, int size){
    int left = here * 2 ;
    int right = here * 2 + 1;
    int max=here;
    if (left<=size&&h->heap[left]->key<h->heap[max]->key)
        max = left;
    if (right <= size&&h->heap[right]->key<h->heap[max]->key)
        max = right;
    
    if (max != here) {
        swap(&h->heap[here], &h->heap[max]);
        heapify(h, max, size);
    }
}

void buildHeap(Heap *h, int size) {
    int i;
    for (i = size / 2 ; i > 0; i--) {
        heapify(h, i, size);
    }
}


void createNode(linkedList *L, int tdata, int index) {
    item *newNode = (item *)malloc(sizeof(item));
    newNode->w = tdata;
    newNode->index = index;
    newNode->next = NULL;
    if(L->head == NULL && L->tail == NULL)
        L->head = L->tail = newNode;
    else {
        L->tail->next = newNode;
        L->tail = newNode;
    }
}


int main(int argc, const char * argv[]) {
    
    FILE* fp = fopen("hw7_data","r");
    if(fp == NULL){
        printf("file open fail\n");
        return -1;
    }
    
    char buffer[256];
    fgets(buffer,256,fp);
    
    remove_space(buffer);
    char newbuffer[256] = {0};
    enter_space(buffer,newbuffer);
    int citynum = number_city(buffer);
    char* city_name[citynum];
    
    char *ptr = strtok(newbuffer, " ");
    int index = 0;
    
    int adjmatrix[citynum][citynum];
    
    
    while (ptr != NULL)
    {
        city_name[index] = ptr;
        index++;
        ptr = strtok(NULL, " ");
    }
    int file_index = 0;
    
    linkedList* adjlist1[citynum];
    for(int i=0;i<citynum;i++){
        adjlist1[i] = (linkedList *)malloc(sizeof(linkedList));
        adjlist1[i]->cur = NULL;
        adjlist1[i]->head = NULL;
        adjlist1[i]->tail = NULL;
    }
    
    while (feof(fp) == 0) {
        if(fgets(buffer,256,fp) == NULL) continue;
        
        for(int i=0;i<(int)strlen(buffer);i++){
            if(isspace(buffer[i])){
               buffer[i] = ' ';
            }
        }
        
        ptr = strtok(buffer, " ");
        ptr = strtok(NULL, " ");
        index = 0;
        char max[5] = "9999";
        
        
        
        while (ptr != NULL)
        {
            if(strcmp(ptr,"INF")==0) ptr = max;
            adjmatrix[file_index][index] = atoi(ptr);
            index++;
            ptr = strtok(NULL, " ");
        }
        
        
        for(int i=0;i<citynum;i++){
            if(adjmatrix[file_index][i] != 0 && adjmatrix[file_index][i] != 9999){
                createNode(adjlist1[file_index],adjmatrix[file_index][i],i);
            }
        }
        file_index++;
    }
    clock_t start1, finish1;
    double duration1=0.0;
    clock_t start2, finish2;
    double duration2=0.0;
    
    //djs 코드 시작
    start1 = clock();
    node Garray[citynum][citynum];
    
    for(int i=0;i<citynum;i++){
        for(int j=0;j<citynum;j++) {
            Garray[i][j].key = 9999;
            Garray[i][j].index = j;
        }
        Garray[i][i].key = 0;
        
        Heap heap1;
        
        for(int j=0;j<citynum;j++){
            insert_min_heap(&heap1,&Garray[i][j]);
        }
        while(heap1.heap_size != 0){
            node* x = delete_min_heap(&heap1);
            item* y = adjlist1[x->index]->head;
            while(y != NULL){
                int w = y->w;
                if(Garray[i][y->index].key > Garray[i][x->index].key + w){
                    Garray[i][y->index].key = (Garray[i][x->index].key + w);
                    buildHeap(&heap1, heap1.heap_size);
                }
                y = y -> next;
            }
        }
    }
    finish1 = clock();
    duration1 = (double)(finish1-start1)/CLOCKS_PER_SEC;
    printf("It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n",duration1);
    printf("\n");
    printf("          ");
    for(int i=0;i<citynum;i++){
        printf("%-10s",city_name[i]);
    }
    printf("\n");
    for(int i=0;i<citynum;i++){
        printf("%-10s",city_name[i]);
        for(int j=0;j<citynum;j++){
            printf("%-10d",Garray[i][j].key);
        }
        printf("\n");
    }
    //djs끝
    printf("\n\n\n");
    //floyd 시작
    start2 = clock();
    int Deven[citynum][citynum];
    int Dodd[citynum][citynum];
    
    for(int i=0;i<citynum;i++){
        for(int j=0;j<citynum;j++){
            Deven[i][j] = adjmatrix[i][j];
        }
    }
    int k;
    for(k=1;k<citynum;k++){
        for(int i=0;i<citynum;i++){
            for(int j=0;j<citynum;j++){
                if(k%2 == 1){
                    if(Deven[i][k] + Deven[k][j] < Deven[i][j]){
                        Dodd[i][j] = Deven[i][k] + Deven[k][j];
                    }
                    else{
                        Dodd[i][j] = Deven[i][j];
                    }
                }
                else{
                    if(Dodd[i][k] + Dodd[k][j] < Dodd[i][j]){
                        Deven[i][j] = Dodd[i][k] + Dodd[k][j];
                    }
                    else{
                        Deven[i][j] = Dodd[i][j];
                    }
                }
            }
        }
    }
    finish2 = clock();
    duration2 = (double)(finish2-start2)/CLOCKS_PER_SEC;
    printf("It took %f seconds to compute shortest path between cities with Floyd algorithm as follows.\n",duration2);
    printf("\n");
    printf("          ");
    for(int i=0;i<citynum;i++){
        printf("%-10s",city_name[i]);
    }
    printf("\n");
    
    if(k % 2 == 1){
        for(int i=0;i<citynum;i++){
            printf("%-10s",city_name[i]);
            for(int j=0;j<citynum;j++){
                printf("%-10d",Deven[i][j]);
            }
            printf("\n");
        }
    }
    
    else{
        for(int i=0;i<citynum;i++){\
            printf("%-10s",city_name[i]);
            for(int j=0;j<citynum;j++){
                printf("%-10d",Dodd[i][j]);
            }
            printf("\n");
        }
    }
    //floyd 끝
    return 0;
}

void remove_space(char* s){
    char* d = s;
    do{
        while(isspace(*d)){
            ++d;
        }
        *s++ = *d++;
    } while(*s != '\0');
}

int number_city(char* s){
    int count = 0;
    for(int i=0;i<(int)strlen(s);i++){
        if(isupper(s[i])){
            count++;
        }
    }
    return count;
}

void enter_space(char* s,char* n){
    do{
        while(isupper(*s)){
            *n++=' ';
            *n++ = *s;
            ++s;
        }
        *n++=*s++;
    } while(*s != '\0');
}

void insert_min_heap(Heap *h, node* item){
  int i;
  i = ++(h->heap_size); // 힙 크기를 하나 증가

  while((i != 1) && (item->key < h->heap[i/2]->key)){
    h->heap[i] = h->heap[i/2];
    i /= 2;
  }
  h->heap[i] = item;
}

node* delete_min_heap(Heap *h){
    int parent, child;
    node* item;
    node* temp;

    item = h->heap[1];
    temp = h->heap[(h->heap_size)--];
    parent = 1;
    child = 2;

    while(child <= h->heap_size){
        if( (child < h->heap_size) && ((h->heap[child]->key) > h->heap[child+1]->key) ){
        child++;
    }
    if( temp->key <= h->heap[child]->key ){
        break;
    }
    h->heap[parent] = h->heap[child];
    parent = child;
    child *= 2;
  }
  h->heap[parent] = temp;
  return item;
}

