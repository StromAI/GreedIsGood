/*
*	Process_Scheduling.c
*	Sliencer
*	12:11:2014
*	A demo from APUE about Process times 
*	It will shows the time of shell cmd
*/ 

#include "apue.h"
#include <sys/times.h>



/*	exit status illustrate*/
void 
pr_exit(int status)
{
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n",
			WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",
			WTERMSIG(status),
#ifdef WCOREDUMP
			WCOREDUMP(ststus) ? " (core file generated)" : "");
#else	
			"");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, single number = %d\n",
			WSTOPSIG(status));			
}

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

int
main(int argc, char *argv[])
{
	int i;

	setbuf(stdout, NULL);
	for (i = 1; i < argc; i++)
		do_cmd(argv[i]);	/*	once for each command-line arg*/
	exit(0);
}

static void
do_cmd(char *cmd)			/*	execute and time the "cmd"	*/
{
	struct tms tmsstart, tmsend;
	clock_t start, end;
	int status;

	printf("\ncommand: %s\n", cmd);

	if ((start = times(&tmsstart)) == -1)	/*	starting values	*/
		err_sys("times error");

	if ((status = system(cmd)) < 0)			/*	execute command */
		err_sys("system() error");

	if ((end = times(&tmsend)) == -1)		/*	ending values	*/
		err_sys("times error");

	pr_times(end-start, &tmsstart, &tmsend);
	pr_exit(status);
}

static void
pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long clktck = 0;

	if (clktck == 0)	/*	fetch clock ticks per second first time */
		if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
			err_sys("sysconf error");

	printf("	real: %7.2f\n",	real / (double) clktck);
	printf("	user: %7.2f\n",
		(tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
	printf("	sys:: %7.2f\n",
		(tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
	printf("	user: %7.2f\n",
		(tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
	printf("	child user: %7.2f\n",
		(tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
	printf("	child sys: %7.2f\n",
		(tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}

/*
 *	Test:
 *	$sudo ./a.out "sleep 5" "date" "man bash >/dev/null"
 *
 *	My out put:
 *	command: sleep 5
 *		real:    5.01
 *		user:    0.00
 *		sys::    0.00
 *		user:    0.00
 *		child user:    0.00
 *		child sys:    0.00
 *	normal termination, exit status = 0
 *
 *	command: date
 *	2014年 12月 13日 星期六 23:20:17 CST
 *		real:    0.00
 *		user:    0.00
 *		sys::    0.00
 *		user:    0.00
 *		child user:    0.00
 *		child sys:    0.00
 *	normal termination, exit status = 0
 *
 *	command: man bash >/dev/null
 *		real:    0.24
 *		user:    0.00
 *		sys::    0.00
 *		user:    0.00
 *		child user:    0.36
 *		child sys:    0.04
 *	normal termination, exit status = 0
 */