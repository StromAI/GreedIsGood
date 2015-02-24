/**
 *	tcpcli01.c
 *	Sliencer
 *	02:02:2015
 *	A demo from UNP
 *	A simple echo client(for ipv4)
 */ 

 #include "../unp.h"
 #include "apue.h"

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;
	ptr = vptr;
	for (n = 1; n < maxlen; n++)
	{
again:
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;
		}		
		else if (rc == 0)
		{
			*ptr = 0;
			return(n -1);
		}
		else
		{
			if (errno == EINTR)
				goto again;
			return(-1);
		}
	}
	*ptr = 0;
		
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
str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while (fgets(sendline, MAXLINE, fp) != NULL)
	{
		if (writen(sockfd, sendline, strlen(sendline)) < 0)
			err_sys("write err");
		printf("send\n");

		if (readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli:server terminated prematurelt");

		fputs(recvline, stdout);
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

 	str_cli(stdin, sockfd);
	exit(0);
}

