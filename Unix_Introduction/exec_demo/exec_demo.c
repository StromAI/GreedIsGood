/*
*	exec_demo.c
*	Use for demonstrate function exec
*/
#include "apue.h"
#include <sys/wait.h>

char *env_init[] = { "USER=unknown", "PATH=/temp", NULL };

int
main (void)
{
	pid_t pid;

	if((pid = fork()) < 0)
	{
		err_sys("fork_error");
	}
	else if (pid == 0)
	{
		if (execle(/* need your absolute pathname here */"echoall", 
					"echoall", "myarg1", "myarg2", (char*)0, env_init) < 0)			/*	specify your pathname & environment	*/
		{
			err_sys("execle_error");
		}
	}

	if (waitpid(pid, NULL, 0) < 0)
		err_sys("wait_error");

	if((pid = fork()) < 0)
	{
		err_sys("fork_error");
	}
	else if (pid == 0)
	{
		if (execlp(/* need your absolute pathname here */"echoall", 
					"echoall", "only 1 arg", (char *)0) < 0)
			err_sys("execlp_error");
	}

	exit(0);
} 
