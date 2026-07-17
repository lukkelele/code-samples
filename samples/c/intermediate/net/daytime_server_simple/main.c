#include "net/net.h"

#include <time.h>

int main(int argc, char** argv)
{
	const int listen_fd = lk_socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13); /* daytime server */

	LOG("bind -> sock %d (port %d)", listen_fd, ntohs(servaddr.sin_port));
	lk_bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	LOG("listen -> sock %d", listen_fd);
	lk_listen(listen_fd, LISTEN_QUEUE_SIZE);

	for (;;) {
		// const int connfd = lk_accept(listenfd, (struct sockaddr*)NULL, NULL);
		struct sockaddr_storage conn_addr;
		socklen_t conn_addr_len = sizeof(conn_addr);
		const int conn_fd = lk_accept(listen_fd, (struct sockaddr*)&conn_addr, &conn_addr_len);

		char client_addr[INET6_ADDRSTRLEN] = {0};
		uint16_t client_port = 0;

		if (conn_addr.ss_family == AF_INET) {
			const struct sockaddr_in* ipv4_addr = (const struct sockaddr_in*)&conn_addr;
			char ipv4_addr_str[INET_ADDRSTRLEN];
			const char* inet_ret = inet_ntop(AF_INET, &ipv4_addr->sin_addr, ipv4_addr_str, sizeof(ipv4_addr_str));
			assert(inet_ret);
			client_port = ntohs(ipv4_addr->sin_port);
			strcpy(client_addr, ipv4_addr_str);
		}

		char buf[MAXLINE];
		const time_t ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
		LOG("write %d bytes -> %s:%u", strlen(buf), client_addr, client_port);
		lk_write(conn_fd, buf, strlen(buf));

		lk_close(conn_fd);
	}
}
