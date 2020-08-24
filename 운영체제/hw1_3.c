#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){

	int sum = 0;
	for (int i=1; i <argc; i++)
	{
		int num = atoi(argv[i]);
		sum += num;
	}
	printf("sum of %d numbers: %d\n",argc-1,sum);
	return 0;

}
