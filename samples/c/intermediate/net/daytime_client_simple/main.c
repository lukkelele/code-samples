#include "net/net.h"

int main(int argc, char** argv)
{
	uint16_t port = 13; /* daytime server */
	if (argc < 2) {
		err_quit("usage: <ip_addr> [port]");
	} else if (argc == 3) {
		port = strtoul(argv[2], NULL, 10);
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
	servaddr.sin_port = htons(port);
	LOG("ip: %s (port %d)", ip_addr, port);
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
