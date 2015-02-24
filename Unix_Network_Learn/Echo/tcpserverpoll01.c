/**
 *	tcpserverpoll01.c
 *	Sliencer
 *	02:24:2015
 *	A demo from UNP
 *	A server use poll in it which without another process
 */ 

#include "../unp.h"
#include "apue.h"
#include <limits.h>			/* for OPEN_MAX */

int
Socket(int family, int type, int protocol)
{
	int		n;

	if ( (n = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return(n);
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

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}

void
Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}

void
Listen(int fd, int backlog)
{
	char	*ptr;

		/*4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}

int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int		n;

	if ( (n = poll(fdarray, nfds, timeout)) < 0)
		err_sys("poll error");

	return(n);
}

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}

void
Close(int fd)
{
	if (close(fd) == -1)
		err_sys("close error");
}

int
main(int argc, char **argv)
{
	int i, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	char buf[MAXLINE];
	socklen_t clilen;
	struct pollfd client[_POSIX_OPEN_MAX];
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for (i = 1; i < _POSIX_OPEN_MAX; i++)
		client[i].fd = -1;
	maxi = 0;

	for (;;)
	{
		nready = Poll(client, maxi + 1, INFTIM);

		if (client[0].revents & POLLRDNORM)
		{
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);

			for (i = 1; i < _POSIX_OPEN_MAX; i++)
				if (client[i].fd < 0)
				{
					client[i].fd = connfd;
					break;
				}
			if (i == _POSIX_OPEN_MAX)
				err_quit("too many clients");

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;

			if (--nready <= 0)
				continue;
		}

		for (i = 1; i <= maxi; i++)
		{
			if ((sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR))
			{
				if ((n = read(sockfd, buf, MAXLINE)) < 0)
				{
					if (errno == ECONNRESET)
					{
						/* connection reset by client */
						Close(sockfd);
						client[i].fd = -1;
					}
					else
						err_sys("read error");
				}
				else if (n == 0)
				{
					/* connection closed by client */
					Close(sockfd);
					client[i].fd = -1;
				}
				else
					Writen(sockfd, buf, n);

				if (--nready <= 0)
					break;			/* no more readable descriptiors */
			}
		}
	}
}
