/**
 *	daytimetcpsrv.c
 *	Sliencer
 *	01:02:2014
 *	A demo from UNP about thread mutex
 *	A simple get-time server(for ipv4)
 */  

#include "../unp.h"
#include "apue.h"		/* About this you can find in Unix_Introduction/README.md */
#include <time.h>

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server*/

	if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("bind err");

	if (listen(listenfd, LISTENQ) < 0)
		err_sys("listen err");

	for ( ; ; )
	{
		if ((connfd = accept(listenfd, (SA *)NULL, NULL)) < 0)
			err_sys("accept err");

		ticks = time(NULL);

		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (write(connfd, buff, strlen(buff)) < 0)
			err_sys("write err");

		if (close(connfd) < 0)
			err_sys("close err");
	}

	exit(0);
}