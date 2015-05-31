/*Max queue*/

#include <stdio.h>
#include <limits.h>

#define PARENT(i) (i/2)
#define LEFT(i) (2*i)
#define RIGHT(i) (2*i+1)

void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void MaxHeapify(int *a, int i, int heapSize)
{
	int l = LEFT(i);
	int r = RIGHT(i);
	int largest;
	if (l <= heapSize && a[l] > a[i])
		largest = l;
	else
		largest = i;
	if (r <= heapSize && a[r] > a[largest])
		largest = r;

	if (largest != i)
	{
		swap(&a[i],&a[largest]);
		MaxHeapify(a,largest,heapSize);
	}
}

void BulidMaxHeap(int *a, int length)
{
	int heapSize = length;
	int i;
	for (i = length / 2; i >= 0; --i)
		MaxHeapify(a,i,heapSize);
}

int HeapMaxMum(int *a)
{
	return a[0];
}

int HeapExtractMax(int *a, int *heapSize)
{
	if (*heapSize < 0)
	{
		printf("error heap overflow\n");
		return INT_MIN;
	}

	int max = a[0];
	a[0] = a[*heapSize - 1];
/*	printf("I'm heapsize %d\n", a[0]);*/
	*heapSize -= 1;
	MaxHeapify(a,0,*heapSize);

	return max;
}

void HeapIncreaseKey(int *a, int i, int key, int heapSize)
{
	if (i >= heapSize)
	{
		printf("The parames \"i\" is too large. \n");
		return;
	}
	if (key < a[i])
	{
		printf("new key is smaller then current key.\n");
		return;
	}
	a[i] = key;
	while (i > 0 && a[PARENT(i)] < a[i])
	{
		swap(&a[i],&a[PARENT(i)]);
		i = PARENT(i);
	}
}

void MaxHeapInsert(int *a, int key, int *heapSize, int length)
{
	if (*heapSize >= length)
	{
		printf("no speace to insert.\n");
		return;
	}
	*heapSize += 1;
	a[*heapSize] = INT_MIN;
	HeapIncreaseKey(a,*heapSize - 1,key,*heapSize);
}

int main(int argc, char const *argv[])
 {
 	int a[100];
 	int i;
 	for (i = 0; i < 10; ++i)
 	{
 		a[i] = i;
 	}
 	printf("----------------------------------------\n");
 	int heapSize = i;
 	for (i = 0; i < 10; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
 	BulidMaxHeap(a,heapSize);
 	for (i = 0; i < 10; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
 	printf("%d\n",HeapExtractMax(a,&heapSize));
 	printf("----------------------------------------\n");
 	for (i = 0; i < heapSize; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
 	HeapIncreaseKey(a, 8, 20, heapSize);
 	for (i = 0; i < heapSize; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
	MaxHeapInsert(a, 19, &heapSize, 100);
 	for (i = 0; i < heapSize; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
 	return 0;
 } 
