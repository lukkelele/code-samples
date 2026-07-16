#include "net.h"

void lk_listen(int fd, int backlog)
{
	char* ptr = NULL;
	if ((ptr = getenv("LISTEN_QUEUE_SIZE")) != NULL) {
		backlog = atoi(ptr);
	}

	if (listen(fd, backlog) < 0) {
		err_sys("listen error");
	}
}

int lk_poll(struct pollfd* fds, unsigned long nfds, int timeout)
{
	int n = -1;
	if ((n = poll(fds, nfds, timeout)) < 0) {
		err_sys("poll error");
	}
	return n;
}

int lk_socket(const int family, const int type, const int protocol)
{
	int n = -1;
	if ((n = socket(family, type, protocol)) < 0) {
		err_sys("failed to create socket (%d)", errno);
	}
	return n;
}

void lk_socketpair(int family, int type, int protocol, int* fd)
{
	int n = -1;
	if ((n = socketpair(family, type, protocol, fd)) < 0) {
		err_sys("socketpair error");
	}
}

int lk_accept(int fd, struct sockaddr* sa, socklen_t* sa_len_ptr)
{
	int sock = -1;
again:
	if ((sock = accept(fd, sa, sa_len_ptr)) < 0) {
#ifdef EPROTO
		if (errno == EPROTO || errno == ECONNABORTED) {
#else
		if (errno == ECONNABORTED) {
#endif
			goto again;
		} else {
			err_sys("accept error");
		}
	}
	return sock;
}

void lk_bind(int fd, const struct sockaddr* sa, socklen_t sa_len)
{
	if (bind(fd, sa, sa_len) < 0) {
		err_sys("bind error");
	}
}

void lk_connect(int fd, const struct sockaddr* sa, socklen_t sa_len)
{
	if (connect(fd, sa, sa_len) < 0) {
		err_sys("connect error");
	}
}

ssize_t lk_recv(int fd, void* ptr, size_t nbytes, int flags)
{
	ssize_t n = -1;
	if ((n = recv(fd, ptr, nbytes, flags)) < 0) {
		err_sys("recv error");
	}
	return n;
}

ssize_t lk_recvfrom(int fd, void* ptr, size_t nbytes, int flags, struct sockaddr* sa, socklen_t* sa_len_ptr)
{
	ssize_t n = -1;
	if ((n = recvfrom(fd, ptr, nbytes, flags, sa, sa_len_ptr)) < 0) {
		err_sys("recvfrom error");
	}
	return n;
}

ssize_t lk_recvmsg(int fd, struct msghdr* msg, int flags)
{
	ssize_t n = -1;
	if ((n = recvmsg(fd, msg, flags)) < 0) {
		err_sys("recvmsg error");
	}
	return n;
}

int lk_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout)
{
	int n = -1;
	if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0) {
		err_sys("select error");
	}
	return n; /* can return 0 on timeout */
}

void lk_send(int fd, const void* data, size_t nbytes, int flags)
{
	if (send(fd, data, nbytes, flags) != (ssize_t)nbytes) {
		err_sys("send error");
	}
}

void lk_sendto(int fd, const void* data, size_t nbytes, int flags, const struct sockaddr* sa, socklen_t sa_len)
{
	if (sendto(fd, data, nbytes, flags, sa, sa_len) != (ssize_t)nbytes) {
		err_sys("sendto error");
	}
}

void lk_sendmsg(int fd, const struct msghdr* msg, int flags)
{
	unsigned int i;
	ssize_t nbytes = 0; /* must first figure out what return value should be */
	for (i = 0; i < msg->msg_iovlen; i++) {
		nbytes += msg->msg_iov[i].iov_len;
	}

	if (sendmsg(fd, msg, flags) != nbytes) {
		err_sys("sendmsg error");
	}
}

void lk_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
	if (setsockopt(fd, level, optname, optval, optlen) < 0) {
		err_sys("setsockopt error");
	}
}

void lk_shutdown(int fd, int how)
{
	if (shutdown(fd, how) < 0) {
		err_sys("shutdown error");
	}
}
