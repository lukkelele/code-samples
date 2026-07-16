#include "net/net.h"

int main(int argc, char** argv)
{
	if (argc != 2) {
		err_quit("missing ip address arg");
	}
	const char* ip_addr = argv[1];
	try_convert_localhost(&ip_addr);

	int sockfd = -1;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_sys("socket error");
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); /* daytime server */
	LOG("ip: %s", ip_addr);
	if (inet_pton(AF_INET, ip_addr, &servaddr.sin_addr) <= 0) {
		err_quit("inet_pton error for %s", ip_addr);
	}

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		err_sys("failed to connect to sock %d", sockfd);
	}

	char recvline[MAXLINE + 1];
	int n = 0;
	int counter = 0;
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++;
		recvline[n] = '\0'; /* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			err_sys("fputs error");
		}
	}
	if (n < 0) {
		err_sys("read error");
	}

	LOG("counter: %d", counter);
	exit(0);
}
