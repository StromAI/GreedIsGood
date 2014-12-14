/*
 *	Simple_Signal_Processing.c
 *	Sliencer
 *	12:12:2014
 *	A demo from APUE about signal 
 *	It will handle the signal SIGUSR1 & SIGUSR2
 */ 
 
#include "apue.h"

static void sig_usr(int);	/* one handler for both signals */

int
main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR2");

	for (;;)
		pause();
}

static void
sig_usr(int signo)	/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		err_dump("received singal %d\n", signo);
}

/*
 *	Test:
 *  $ ./a.out &
 *  [1] 9912
 *  $ kill -USR1 9912
 *  received SIGUSR1
 *  $ kill -USR2 9912
 *  received SIGUSR2
 *  $ kill 9912
 *  [1]+  已终止               ./a.out 
 */