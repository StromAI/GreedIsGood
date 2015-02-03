/**
 *	daytimetcpsrv.c
 *	Sliencer
 *	02:02:2015
 *	A demo from UNP
 *	A simple echo server(for ipv4)
 *	Of course this server will cause zombie process we must deal with it
 */ 

#include "../unp.h"
#include "apue.h"

pid_t 
Fork()
{
	pid_t pid;

	if ((pid = fork()) == -1)
		err_sys("fork err");

	return pid;
}

int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int		n;

again:
	if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}

ssize_t
writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) < 0 )
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return(-1);
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return(n);
}

void
str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];

	printf("child first\n");
again:
	while ((n = read(sockfd, buf, MAXLINE)) > 0)
	{

		printf("child n:%d\n", n);

		if (writen(sockfd, buf, n) < 0)
			err_sys("write err");

		if (n < 0 && errno == EINTR)
			goto again;
		else if (n < 0)
			err_sys("str_echo: read error");

	}
	printf("child second\n");
}

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	char buff[MAXLINE];

	if ((listenfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
		err_sys("socket err");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("bind err");

	if (listen(listenfd, LISTENQ) < 0)
		err_sys("listen err");

	for ( ; ; )
	{
		clilen = sizeof(cliaddr);

		connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

		printf("connection from %s, port %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			ntohs(cliaddr.sin_port));

		if ((childpid = Fork()) == 0)
		{
			if (close(listenfd) < 0)
				err_sys("close err");

			str_echo(connfd);
			exit(0);
		}
		if (close(connfd) < 0)
			err_sys("close err");		
	}
} 
