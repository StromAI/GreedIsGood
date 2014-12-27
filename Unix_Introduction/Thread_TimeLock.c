/*
 *	Thread_TimeLock.c
 *	Sliencer
 *	12:27:2014
 *	A demo from APUE about thread mutex time lock
 *	Not a good demo but it shows how pthread_mutex_timedlock work
 *	such function will wait till time out and return the err number
 *	or lock the resurce successful.
 */ 

#include "apue.h"
#include <pthread.h>

int
main(void)
{
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time is %s \n", buf);

	tout.tv_sec += 10;	/* 10 second from now */
	/*	caution: this could lead to dead lock */
	err = pthread_mutex_timedlock(&lock, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("the time is now %s\n", buf);
	if (err == 0)
		printf("mutex locked again!\n");
	else
		printf("can't lock mutex again: %s\n", strerror(err));
	exit(0);
}

/*
 *	$ ./ttl.out
 *	mutex is locked
 *	current time is 09:44:39 PM 
 *	the time is now 09:44:49 PM
 *	can't lock mutex again: Connection timed out
 */