#include <stdio.h>
#include <limits.h>
#include <malloc.h>

#define false 0
#define true 1
#define bool int

void Marge(int *a, int p, int q, int r)
{
	int n1 = q - p;
	int n2 = r - q + 1;
	
	int *left = NULL;
	left = (int*)malloc(sizeof(int)*(n1 + 1));
	int *right = NULL;
	right = (int*)malloc(sizeof(int)*(n2 + 1));
	
	int i;
	for (i = 0; i < n1; ++i)
		left[i] = a[p + i];

	int j;
	for (j = 0; j < n2; ++j)
		right[j] = a[q + j];

	left[n1] = INT_MAX;
	right[n2] = INT_MAX;
	i = 0;
	j = 0;

	int k;
	for (k = p; k < r; ++k)
	{
		if (left[i] <= right[j])
		{
			a[k] = left[i];
			++i;
		}
		else
		{
			a[k] = right[j];
			++j;
		}
	}
	free(left);
	free(right);
}

void MargeSort(int *a, int p, int r)
{
	if (p < r)
	{
		int q = (p+r) / 2;
		MargeSort(a,p,q);
		MargeSort(a,q + 1,r);
		Marge(a,p,q,r);
	}
}

bool BinarySearch(int *a, int length, int x)
{
	int low = 0;
	int hight = length -1;
	while (low <= hight)
	{
		int mid = (low + hight) / 2;
		if (a[mid] == x)
			return true;
		else if (a[mid] > x)
			hight = mid - 1;
		else
			low = mid + 1;
	}
	return false;
}

bool SumSearch(int *a, int length, int x)
{
	MargeSort(a,0,length);
	int i;
	/*for (i = 0; i < length; ++i)
	{
		printf("%d\n",a[i]);
	}
	printf("-----------------------------------\n");*/
	for (i = 0; i < length; ++i)
	{
		if (BinarySearch(a, length, x - a[i]))
		{
			return true;
		}
	}
	return false;
}

int main(int argc, char const *argv[])
{
	int a[10] = {1,2,3,4,5,6,7,8,9,0};
	printf("%d\n",SumSearch(a,10,9));
	printf("%d\n",SumSearch(a,10,20));
	return 0;
}
