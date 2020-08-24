#include<stdio.h>
#include<stdlib.h>


int combination1(int n, int r){

	if(r > n || n == 0)	return 0;
	else if(n==r || r==0)	return 1;

	return combination1(n-1,r-1) + combination1(n-1, r);

}

int combination2(int n, int r){

	int arr[100][100];

	int i,j;
	// nCr = (n-1)C(r-1) + (n-1)C(r)
	for(i=0;i<=n;i++){
		for(j=0; j<=r;j++){

			if (i==0)
				arr[i][j] =0;
			else if(i==j || j==0)
				arr[i][j] = 1;
			else
				arr[i][j] = arr[i-1][j-1] + arr[i-1][j];
		}
	}

	return arr[n][r];

}

int main(int argc, char* argv[])
{
  int n;
  int r;
  if(argc == 3){
    n = atoi(argv[1]);
    r = atoi(argv[2]);
  }
  else{
    printf("enter value of n : ");
    scanf("%d",&n);
    printf("enter value of r : ");
    scanf("%d",&r);
  }

	if (r>n) {
		printf("r have to be lower than n\n");
  	return 0;
	}
	printf("dp : %d \n",combination2(n,r));
  printf("rc : %d \n",combination1(n,r));

	return 0;
}
