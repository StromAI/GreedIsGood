 
/**
 *	tcpcli03.c
 *	Sliencer
 *	02:24:2015
 *	A demo from UNP
 *	A simple echo client(for ipv4) use select to handle 
 *	the situations when server is close and would consider 
 *	about the batch jobs
 */ 

 #include "../unp.h"
 #include "apue.h"

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		err_sys("read error");
	return(n);
}

void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}

void
Shutdown(int fd, int how)
{
	if (shutdown(fd, how) < 0)
		err_sys("shutdown error");
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		err_sys("select error");
	return(n);		/* can return 0 on timeout */
}

void
str_cli(FILE *fp, int sockfd)
{
	int maxfdp1, stdineof;
	fd_set rset;
	char buf[MAXLINE];
	int n;

	stdineof = 0;
	FD_ZERO(&rset);
	for(;;)
	{
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset))
		{
			if ((n = Read(sockfd, buf, MAXLINE)) == 0)
			{
				if (stdineof == 1)
					return;
				else
					err_quit("str_cli: server terminated prematurely");
			}
			Write(fileno(stdout), buf, n);
		}
		if (FD_ISSET(fileno(fp), &rset))
		{
			if ((n = Read(fileno(fp), buf, MAXLINE)) == 0)
			{
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR);	/*	send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			Write(sockfd, buf, n);
		}
	}
}

int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2)
 		err_quit("usage: %s <IPaddress>", argv[0]);

 	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
 		err_sys("socket error");

 	bzero(&servaddr, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_port = htons(SERV_PORT);
 	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
 		err_quit("inet_pton error for %s", argv[1]);

 	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
 		err_sys("connect err");

 	//FILE* fp;
 	//fp = fopen("/home/sliencer/CodeDump/GreedIsGood/Unix_Network_Learn/Echo/tcpcli01.c", "r");

 	str_cli(stdin, sockfd);
	exit(0);
}

 
