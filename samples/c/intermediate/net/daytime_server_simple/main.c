#include "net/net.h"

#include <time.h>

int main(int argc, char** argv)
{
	const int listenfd = lk_socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13); /* daytime server */

	LOG("bind -> sock %d (port %d)", listenfd, ntohs(servaddr.sin_port));
	lk_bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	LOG("listen -> sock %d", listenfd);
	lk_listen(listenfd, LISTEN_QUEUE_SIZE);

	for (;;) {
		const int connfd = lk_accept(listenfd, (struct sockaddr*)NULL, NULL);

		char buf[MAXLINE];
		const time_t ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
		lk_write(connfd, buf, strlen(buf));

		lk_close(connfd);
	}
}
