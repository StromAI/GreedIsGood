#include <stdio.h>

void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int Partition(int *a, int p, int r)
{
	int x = a[r];
	int i = p - 1;
	int j;
	for (j = p; j < r; ++j)
	{
		if (a[j] < x)
		{
			++i;
			swap(&a[i], &a[j]);
		}
	}
	swap(&a[i+1], &a[r]);
	return i+1;
}

void QuckSort(int *a, int p, int r)
{
	if (p < r)
	{
		int q = Partition(a, p ,r);
		QuckSort(a,p,q-1);
		QuckSort(a,q+1,r);
	}
}


int main(int argc, char const *argv[])
 {
 	int a[10] = {1,4,5,6,2,2,345,6,10,19};
 	int i;
 	printf("----------------------------------------\n");
 	for (i = 0; i < 10; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	printf("----------------------------------------\n");
 	QuckSort(a,0,9);
 	for (i = 0; i < 10; ++i)
 	{
 		printf("%d \n", a[i]);
 	}
 	return 0;
 } 
