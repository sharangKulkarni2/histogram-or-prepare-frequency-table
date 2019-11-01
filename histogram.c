#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#define SIZE 10000
#define NUMRANGE 50 
#define NUM_THREADS 1000

int main(){
int i;
int index;
int *arr = (int *)malloc(sizeof(int) * SIZE);
int *hTable = (int *)calloc(NUMRANGE, sizeof(int));
double start,end;
srand(0);

#pragma omp parallel num_threads(NUM_THREADS)
{
#pragma omp for schedule(static, 1)   //cyclic distribution only
for(i =0;i< SIZE;i++){
	arr[i] = rand() % NUMRANGE;
	//printf("%d is my thread number and i is %d and arr[i] is %d\n", 
		//omp_get_thread_num(),i, arr[i]);

  }
}
start = omp_get_wtime();
omp_lock_t writelock;
omp_init_lock(&writelock);
#pragma omp parallel num_threads(NUM_THREADS)
{
#pragma omp for schedule(static, 1)
for(i = 0; i<SIZE; i++){
	index = arr[i];
	omp_set_lock(&writelock);
	hTable[index] = hTable[index] + 1; // this is a 'race stmt'
	omp_unset_lock(&writelock);
}
	omp_destroy_lock(&writelock);
}

end = omp_get_wtime();
printf("%lf", (end-start));

/*
for(int i=0;i<NUMRANGE;i++){
printf("value is %d and count is %d\n", i, hTable[i]);
}
*/


return 0;

}
	