//21600331 배한재 알고르듬분석 2분분
//greedy와 dp만 구현하였습니다.

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

typedef struct _item
{
  int benefit;
  int weight;
  double b_per_w;
} item;

int make_random_benefit();
int make_random_weight();

void mergeSort(item* data, int p, int r);
void merge(item* data, int p, int q, int r);

//sort시 사용되는 global variable
item* tmp;

double greedy(item* itemarray, int limit_weight, int input_size);
int dp(item* itemarray, int limit_weight, int input_size);
int bb(item* itemarray, int limit_weight, int input_size);

typedef struct _node{
  int index;
  int benefit;
  int weight;
  double bound;
} node;

typedef struct _pq{
  node* heap;
  int size;
  int position;
} pq;

void heap_swap(node* a, node* b);
int enqueue(pq* q,node value);
node dequeue(pq* q);

////////////////////////////////////////////////////////////////////////////////

int main()
{
  FILE *fp = fopen("output.txt", "w");

  fprintf(fp,"items\t\t(greedy)\t,\t(dp)\n");

  for(int a=0; a < 9; a++)
  {
  int array[9] = {10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000};

  int input_size = array[a];

  item* itemarray;
  itemarray = (item*)malloc(sizeof(item)*input_size);
  tmp = (item*)malloc(sizeof(item)*input_size);

  srand(time(NULL));
  int i;
  for(i=0;i<input_size;i++)
  {
    int benefit = make_random_benefit();
    int weight = make_random_weight();
    (itemarray+i)->benefit = benefit;
    (itemarray+i)->weight = weight;
    (itemarray+i)->b_per_w = (double)benefit/weight;
  }
  mergeSort(itemarray,0,input_size-1);

  //디버깅
  // printf("#################itmes##############\n");
  // for(i=0;i<input_size;i++){
  //   printf("%d %d %lf\n",(itemarray+i)->benefit,(itemarray+i)->weight,(itemarray+i)->b_per_w);
  // }
  // printf("####################################\n");


  int limit_weight = 40 * input_size;

  fprintf(fp,"n = %d\t\t",array[a]);
  input_size = array[a];

  time_t start1,end1;
  start1 = clock();
  double greedy_result = greedy(itemarray,limit_weight,input_size);
  end1 = clock();
  int clock_result1 = end1 - start1;
  fprintf(fp,"(%d / %.3lf)\t,\t", clock_result1, greedy_result);

  time_t start2,end2;
  start2 = clock();
  int dp_result = dp(itemarray,limit_weight,input_size);
  end2 = clock();
  int clock_result2 = end2 - start2;
  fprintf(fp,"(%d / %d)\n", clock_result2, dp_result);

  // time_t start3,end3;
  // start3 = clock();
  // int bb_result = bb(itemarray,limit_weight,input_size);
  // end3 = clock();
  // int clock_result3 = end3 - start3;
  // printf("%d / %d\n", clock_result3,bb_result);

  free(itemarray);
  free(tmp);

  }

  fclose(fp);


  return 0;
}

////////////////////////////////////////////////////////////////////////////////

int make_random_benefit()
{
  int benefit = rand() % 300 + 1;
  return benefit;
}

int make_random_weight()
{
  int weight = rand() % 100 + 1;
  return weight;
}

void mergeSort(item* data, int p, int r) {
    int q;
    if(p<r) {
        q = (p+r)/2;
        mergeSort(data, p , q);
        mergeSort(data, q+1, r);
        merge(data, p, q, r);
    }
}
void merge(item* data, int p, int q, int r) {
    int i = p, j = q+1, k = p;
    while(i<=q && j<=r) {
        if(data[i].b_per_w >= data[j].b_per_w) tmp[k++] = data[i++];
        else tmp[k++] = data[j++];
    }
    while(i<=q) tmp[k++] = data[i++];
    while(j<=r) tmp[k++] = data[j++];
    for(int a = p; a<=r; a++) data[a] = tmp[a];
}

double greedy(item* itemarray, int limit_weight, int input_size)
{
  int total_weight = 0;
  int total_benefit = 0;
  double result;
  int i = 0;

  while(total_weight + itemarray[i].weight < limit_weight && i < input_size)
  {
    total_weight += itemarray[i].weight;
    total_benefit += itemarray[i].benefit;
    i++;
  }

  result = total_benefit + (limit_weight-total_weight)*itemarray[i].b_per_w;

  return result;
}

int dp(item* itemarray, int limit_weight, int input_size)
{
  int** dparray;
  dparray = (int**)malloc(sizeof(int*)*(limit_weight+1));
  int i;
  for(i=0;i<limit_weight+1;i++){
    dparray[i] = (int*)malloc(sizeof(int)*(input_size+1));
  }



  for(int l=0;l<=limit_weight;l++) dparray[l][0] = 0;

  int j;
  for(j=1;j<=input_size;j++)
  {
    dparray[0][j] = 0;
    int k;
    for(k=1;k<=limit_weight;k++)
    {

      if(itemarray[j-1].weight <= k)
      {
        if(itemarray[j-1].benefit + dparray[k-itemarray[j-1].weight][j-1] > dparray[k][j-1])
        {
          dparray[k][j] = itemarray[j-1].benefit + dparray[k-itemarray[j-1].weight][j-1];
        }
        else dparray[k][j] =  dparray[k][j-1];
      }
      else
      {
        dparray[k][j] = dparray[k][j-1];
      }
    }
  }

  int result = dparray[limit_weight][input_size];

  for(int i=0; i<limit_weight+1; i++){
    free(dparray[i]);
  }
  free(dparray);

  return result;
}

void heap_swap(node* a, node* b){
  node temp = *a;
  *a = *b;
  *b = temp;
}

int enqueue(pq* q,node value){

  // if (q->position + 1 >= q->size){
  //   q->size *= 2;
  //   realloc(q->heap,sizeof(node)*q->size);
  //   //디버깅
  //   printf("size up to %d\n",q->size);
  // }

  q->position += 1;

  q->heap[q->position] = value;

  int current = q->position;
  int parent = q->position/2;


  while (current > 1 && q->heap[current].bound > q->heap[parent].bound){
    heap_swap(&q->heap[current],&q->heap[parent]);
    current = parent;
    parent = current/2;
  }

  return 1;
}

node dequeue(pq* q) {
  // if (q->position  - 1 <= q->size/4 - 1){
  //   q->size /= 2;
  //   realloc(q->heap,sizeof(node)*q->size);
  //   //디버깅
  //   printf("size down to %d\n",q->size);
  // }

  if(q->position <= 0) {
    node temp;
    temp.benefit = -1;
    return temp;
  }

  node ret = q->heap[1];
  q->position -= 1;

  q->heap[1] = q->heap[q->position+1];
  int current = 1;
  int leftChild = current * 2;
  int rightChild = current * 2 + 1;
  int maxNode = current;

  while(leftChild < q->position)
  {
    if(q->heap[maxNode].bound < q->heap[leftChild].bound)
    {
      maxNode = leftChild;
    }
    if(rightChild < q->position && q->heap[maxNode].bound < q->heap[rightChild].bound) {
      maxNode = rightChild;
    }

    if (maxNode == current){
      break;
    }

    else {
      heap_swap(&q->heap[current], &q->heap[maxNode]);
      current = maxNode;
      leftChild = current * 2;
      rightChild = current * 2 + 1;
    }
  }

  return ret;
}

int bb(item* itemarray, int limit_weight, int input_size)
{
  pq q;

  q.size = 2;
  q.heap = (node*)malloc(sizeof(node)*10000);
  q.position = 0;

  double max_benefit = 0.0;
  int weight = 0;

  node root;

  root.weight = 0;
  root.benefit = 0;
  root.index = 0;
  root.bound = 0;

  for(int i = 0; i < input_size; i++){
    if(weight+itemarray[i].weight > limit_weight)
    {
      root.bound += (limit_weight-weight)*itemarray[i].b_per_w;
      break;
    }
    else
    {
      root.bound += itemarray[i].benefit;
      weight += itemarray[i].weight;
    }
  }
  enqueue(&q,root);

  while(1)
  {

    node temp = dequeue(&q);
    //디버깅
    // printf("dequeue : ");
    // printf("%d %d %d %lf %d position : %d\n",temp.index, temp.benefit,temp.weight,temp.bound,(int)max_benefit,q.position);
    node child[2];
    if(temp.benefit == -1) break;

    int index = temp.index + 1;

    if(temp.bound < max_benefit) continue;

    if(temp.index == input_size-1 ) continue;

    child[0].weight = temp.weight + itemarray[index-1].weight;
    child[0].benefit = temp.benefit + itemarray[index-1].benefit;
    child[1].benefit = temp.benefit;
    child[1].weight = temp.weight;


    for(int i = 0; i < 2; i ++)
      {

      child[i].index = index;
      child[i].bound = (double)child[i].benefit;


      if(limit_weight<child[i].weight)
      {
        child[i].bound = 0;
        continue;
      }
      weight = child[i].weight;
      for(int j=index; j<input_size; j++){
        if(weight+itemarray[j].weight > limit_weight){
          child[i].bound += (limit_weight-weight)*itemarray[j].b_per_w;
          break;
        }
        else{
          child[i].bound += itemarray[j].benefit;
          weight += itemarray[i].weight;
        }
      }

      if(child[i].bound < max_benefit) continue;

      if(child[i].benefit > max_benefit) max_benefit = child[i].benefit;

      enqueue(&q,child[i]);
      //디버깅
      // printf("enqueue : ");
      // printf("%d %d %d %lf %d postion : %d\n",child[i].index, child[i].benefit,child[i].weight,child[i].bound,(int)max_benefit,q.position);

    }
  }

  free(q.heap);

  return (int)max_benefit;
}
