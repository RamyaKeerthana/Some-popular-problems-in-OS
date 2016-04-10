#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
void mergesort(int a[],int n);
void merge(int *left, int llength, int *right, int rlength);
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;
 
        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}
int main()
{
	int n;
	printf("Number of values:\n");
	scanf("%d",&n);
	int *shm_array;
	int shm_id;
	key_t key;
	size_t shm_size;
	int a[n];
	printf("Values:\n");
	int i;
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	key=IPC_PRIVATE;
	shm_size = n * sizeof(int);
	if ((shm_id = shmget(key, shm_size, IPC_CREAT | 0666)) == -1)
	{
		perror("shmget");
		exit(1);
	}
	if ((shm_array = shmat(shm_id, NULL, 0)) == (int *) -1) 
	{
		perror("shmat");
		exit(1);
	}
	for (i = 0; i < n; i++) 
	{
		shm_array[i] = a[i];
	}

	mergesort(shm_array,n);
	for(i=0;i<n;i++)
	{
	printf("%d ",shm_array[i]);
	}
	printf("\n");






	if (shmdt(shm_array) == -1)
	{
		perror("shmdt");
		exit(1);
	}
	if (shmctl(shm_id, IPC_RMID, NULL) == -1)
	{	
		perror("shmctl");
		exit(1);
	}
	return 0;
}
void mergesort(int a[],int n)
{
	int mid=n/2;
	int *left,*right,left_child,right_child,status;

	if (n <= 1)
		return;
	if(n<=5)
	{
		selectionSort(a, n);
		return;
	}	
	int i;
	left=a;
	right=a+(n-mid);
	left_child = fork();
	if (left_child < 0)
	{
		perror("fork");
		exit(1);
	}
	if (left_child == 0) 
	{
		mergesort(left,n-mid);
		exit(0);
	}
	else
	{
		right_child = fork();
		if (right_child < 0)
		{
			perror("fork");
			exit(1);
		}
		if (right_child == 0)
		{
			mergesort(right, mid);
			exit(0);
		}
	}
	waitpid(left_child, &status, 0);
	waitpid(right_child, &status, 0);
	merge(left, n-mid, right, mid);
}
void merge(int *lchild, int llength, int *rchild, int rlength)
{
	int *ltmp = (int *) malloc(llength * sizeof(int));
	int *rtmp = (int *) malloc(rlength * sizeof(int));
	int *ll = ltmp;
	int *rr = rtmp;
 
	int *result = lchild;

	memcpy(ltmp, lchild, llength * sizeof(int));
	memcpy(rtmp, rchild, rlength * sizeof(int));

	while (llength > 0 && rlength > 0) {
		if (*ll <= *rr) {
			*result = *ll;
			ll=ll+1;
			llength=llength-1;
		} else {
			*result = *rr;
			rr=rr+1;
			rlength=rlength+1;
		}
		result++;
	}
	if (llength > 0)
		while (llength > 0) {
			*result = *ll;
			result++;
			ll++;
			llength--;
		}
	else
		while (rlength > 0) {
			*result = *rr;
			result++;
			rr++;
			rlength--;
		}

	free(ltmp);
	free(rtmp);
}



