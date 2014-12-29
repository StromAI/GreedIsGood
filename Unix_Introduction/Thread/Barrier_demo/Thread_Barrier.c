/*
 *	Thread_RWLock.c
 *	Sliencer
 *	12:28:2014
 *	A demo from APUE about barrier
 *	Such demo use 8 thread to sort 8000000 numbers,each thread
 *	dealing with 1000000 numbers.In the demo we use barrier to 
 *	wait every thread till them end. Then use marge sort marge
 *	datas.
 *	The result is shows in the other file(out.txt) which
 *	shows us the used time and sort list; 
 */ 
#include "apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 	8				/* number of thread */
#define NUMNUM 	8000000L		/* number of numbers to sort */
#define TNUM 	(NUMNUM/NTHR)	/* number of sort per thread */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

/*	
 *	There is no heapsort at all in my system,so we only define qsort to heapsort
 *
 *	#ifdef SOLARIS
 *	#define heapsort qsort
 *	#else
 *	extern int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));
 *	#endif
 */

#define heapsort qsort

/*
 *	Compare two long integers (helper function for heapsort)
 */

int
complong(const void *arg1, const void *arg2)
{
	long l1 = *(long *)arg1;
	long l2 = *(long *)arg2;

	if (l1 == l2)
		return 0;
	else if (l1 < l2)
		return -1;
	else
		return 1;
}

/*
 *	Worker thread to sort a protion of the set of numbers.
 */
void *
thr_fn(void *arg)
{
	long idx = (long)arg;

	heapsort(&nums[idx], TNUM, sizeof(long), complong);
	pthread_barrier_wait(&b);

	/*
	 *	Go for perform more work ...
	 */

	return((void *)0);
}

/*
 *	Merge the results of the indvidual sorted ranges.
 *	Note for myself: Marge sort here, to marge the result from threads
 */
void 
marge()
{
	long idx[NTHR];
	long i, minidx, sidx, num;

	for (i = 0; i < NTHR; i++)
		idx[i] = i * TNUM;
	for (sidx = 0; sidx < NUMNUM; sidx++)
	{
		num = LONG_MAX;
		for (i = 0; i < NTHR; i++)
		{
			if ((idx[i] < (i + 1)*TNUM) && (nums[idx[i]] < num))
			{
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}

int
main()
{
	unsigned long 	i;
	struct timeval 	start, end;
	long long 		startusec, endusec;
	double 			elapsed;
	int 			err;
	pthread_t 		tid;

	/*
	 *	Create the initial set of number to sort.
	 */
	srandom(1);
	for (i = 0; i < NUMNUM; i++)
		nums[i] = random();

	/*
	 *	Create 8 thread to sort the numbers.
	 */
	gettimeofday(&start, NULL);
	pthread_barrier_init(&b, NULL, NTHR + 1);
	for (i = 0; i < NTHR; i++)
	{
		err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
		if (err != 0)
			err_exit(err, "can't create thread");
	}
	pthread_barrier_wait(&b);
	marge();
	gettimeofday(&end, NULL);

	/*
	 *	Print the sorted list.
	 */

	startusec = start.tv_sec * 1000000 + start.tv_usec;
	endusec = end.tv_sec * 1000000 + end.tv_usec;
	elapsed = (double)(endusec - startusec) / 1000000.0;
	printf("sort took %.4f second\n", elapsed);
	for (i = 0; i < NUMNUM; i++)
		printf("%ld\n", snums[i]);
	exit(0);

}