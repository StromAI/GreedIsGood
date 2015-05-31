#include <stdio.h>


/*
 *	find the kth largest element in the two list
 *	input list a, list b, length of a, length of b ,k(k > 0)
 *	return the element of kth
 *	a & b are decrease
 */
int BinaryKFind(int *a, int *b,int lenghta, int lenghtb, int k)
{
	int lowa = 1;
	int highta = lenghta;
	int lowb = 1;
	int hightb = lenghtb;

	int mida;
	int midb;

	int n = 20;
	while (n--)
	{
		if (highta != -1)
			mida = (lowa + highta) / 2;
		else
			mida = -1;
		if (hightb != -1)
			midb = (lowb + hightb) / 2;
		else
			midb = -1;
		
		///printf("midA = %d,minB = %d\n",mida,midb);
		if ((mida + midb) == k)
		{
			if (mida == -1)
				return b[midb - 1];
			if (midb == -1)
				return a[mida - 1];

			if (a[mida - 1] >= b[midb - 1])
				return b[midb - 1];
			else
				return a[mida - 1];
		}
		else if ((mida + midb + 2) > k)
		{
			if (a[mida] >= b[midb - 1])
			{
				if (highta > lowa)
					hightb = midb - 1;
				else
					highta = mida - 1;
			}
			else
			{
				if (hightb > lowb)
					highta = mida - 1;
				else
					hightb = midb - 1;
			}
		}
		else
		{
			if (a[mida - 1] >= b[midb - 1])
			{
				if (highta >= lowa)
					lowa = mida + 1;
			}
			else
			{
				if (hightb >= lowb)
					lowb = midb + 1;	
			}
		}
	}
} 

int main(int argc, char const *argv[])
{
	int a[5] = {10,7,4,3,0};
	int b[6] = {20,18,9,4,2,1};
	int i;
		printf("%d\n", BinaryKFind(a,b,5,6,8));
	return 0;
}
