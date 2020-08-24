#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define b_size 100

int isspace_(int c);
void remove_space(char* s);

int main(int argc, const char * argv[])
{
    // 파일 open()
    FILE* fp;
    fp = fopen("hw6_.data.txt","r");
    if(fp == NULL)
    {
        printf("IOerror\n");
        return -1;
    }

    //파일 읽어서 노드의 수와 어떤 알파벳으로 구성되어있는지 읽고 2차원 배열 만들기
    char string_buffer[b_size];

    //1.첫줄읽기
    fgets(string_buffer,b_size,fp);
    //2.tab키와 space제거
    char* ptr = string_buffer;
    remove_space(ptr);
    //3.사이즈 구하기
    int osize = 0;
    while(isalpha(*ptr))
    {
        ptr++;
        osize++;
    }
    //4.알파벳 구성을 배열로 저장
    char alpabat_array[osize];
    for(int z=0; z<osize; z++)
    {
        alpabat_array[z] = string_buffer[z];
    }

    //5.파일에서 얻은 정보를 2차원 배열로 가져오기
    int metrix[osize][osize];
    char zoer_or_one[2];
    for(int i=0;i<osize;i++)
    {
        for(int j=0;j<osize+1;j++)
        {
            fscanf(fp,"%s",zoer_or_one);
            if(j==0) continue;
            metrix[i][j-1] = atoi(zoer_or_one);
        }
    }

    //파일닫기
    fclose(fp);

    //adjacency list 만들기
    //1.리스트 선언
    int* ad_list[osize];
    //2.리스트의 각 행의 사이즈를 저장하는 배열 선언
    int ad_list_size[osize];
    //3.사이즈 설정
    for(int i=0;i<osize;i++)
    {
        ad_list_size[i] = 0;
        for(int j=0;j<osize;j++)
        {
            if(metrix[i][j] == 1)
            {
                ad_list_size[i]++;
            }
        }
    }
    //4.구한 사이즈를 이용하여서 adjacency list 할당 후 값 채우기
    for(int i=0;i<osize;i++)
    {
        ad_list[i] = (int*)malloc(sizeof(int)*ad_list_size[i]);
        int  k = 0;
        for(int j=0;j<osize;j++)
        {
            if(metrix[i][j] == 1)
            {
                ad_list[i][k] = j;
                k++;
            }
        }
    }

    //adjacency list 출력
    printf("adjacency list :\n");
    for(int i=0;i<osize;i++)
    {
        printf("V %c --> ",alpabat_array[i]);
        for(int j=0;j<ad_list_size[i];j++)
        {
            printf("%c ",alpabat_array[ad_list[i][j]]);
        }
        printf("\n");

    }

    //transpose 된 adjacency list만들기
    //1.t_ad_size[]를 생성
    int t_ad_size[osize];
    //2.t_ad_size를 초기화
    for(int i=0;i<osize;i++)
        {
            t_ad_size[i] = 0;
        }
    //3.먼저 만든 adlist에서 각 노드가 몇번 나오는지 확인한다.
    for(int i=0;i<osize;i++)
    {
        for(int j=0;j<ad_list_size[i];j++)
        {
            t_ad_size[ad_list[i][j]]++;
        }
    }
    //4.새로 만들 t_ad_list가 어디까지 채워졌는지 기록하는 배열 t_ad_list_capacity 선언
    int t_ad_list_capacity[osize];
    //5.capacity 초기화
    for(int i=0;i<osize;i++)
    {
        t_ad_list_capacity[i] = 0;
    }
    //6.t_ad_list[osize] 선언 및 할당
    int* t_ad_list[osize];
    for(int i=0;i<osize;i++)
    {
        t_ad_list[i] = (int*)malloc(sizeof(int)*t_ad_size[i]);
    }
    for(int i=0;i<osize;i++)
    {
        for(int j=0;j<ad_list_size[i];j++)
        {
            t_ad_list[ad_list[i][j]][t_ad_list_capacity[ad_list[i][j]]] = i;
            t_ad_list_capacity[ad_list[i][j]]++;
        }
    }

    //transpose adjacency list 출력
    printf("\ntranspose adjacency list :\n");
    for(int i=0;i<osize;i++)
        {
            printf("V %c --> ",alpabat_array[i]);
            for(int j=0;j<t_ad_size[i];j++)
            {
                printf("%c ",alpabat_array[t_ad_list[i][j]]);
            }
            printf("\n");
        }

    return 0;
}




//space 와 tab을 찾는 함수
int isspace_(int c)
{
    if( c == ' ' || c == '\t') return 1;
    else return 0;
}
//화이트스페이스 제거 함수
void remove_space(char* s){
    char* d = s;
    do{
        while(isspace_(*d)){
            ++d;
        }
        *s++ = *d++;
    } while(*s != '\0');
}
