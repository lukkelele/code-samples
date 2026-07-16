#include "net.h"

#include <assert.h>
#include <stdarg.h>
#include <syslog.h>

int daemon_proc; /* set nonzero by daemon_init() */

static void flush_log_msg(int level, FILE* fd, const char* buf)
{
	if (daemon_proc) {
		syslog(level, "%s", buf);
	} else {
		fflush(stdout); /* incase stdout and stderr are the same */
		fputs(buf, fd);
		fflush(fd);
	}
}

static void print_stdout(const int level, const char* const fmt, va_list ap)
{
	char buf[MAXLINE + 1];
	vsnprintf(buf, MAXLINE, fmt, ap);
	const int n = strlen(buf);
	strcat(buf, "\n");
	flush_log_msg(level, stdout, buf);
}

static void print_stderr(const int err, const int level, const char* const fmt, va_list ap)
{
	char buf[MAXLINE + 1];
	const int errno_saved = errno;

	vsnprintf(buf, MAXLINE, fmt, ap);
	const int n = strlen(buf);
	if (err) {
		snprintf(buf + n, MAXLINE - n, ": %s (err %d)", strerror(errno_saved), errno_saved);
	}
	strcat(buf, "\n");
	flush_log_msg(level, stderr, buf);
}

void log_msg(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stdout(LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/**
 * @brief Nonfatal error related to system call.
 */
void err_ret(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stderr(1, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/**
 * @brief Fatal error related to system call.
 */
void err_sys(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stderr(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/**
 * @brief Fatal error related to system call.
 */
void err_dump(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stderr(1, LOG_ERR, fmt, ap);
	va_end(ap);
	abort(); /* dump core and terminate */
	exit(1); /* shouldn't get here */
}

/**
 * @brief Fatal error unrelated to system call.
 */
void err_msg(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stderr(0, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/**
 * @brief Fatal error unrelated to system call.
 */
void err_quit(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	print_stderr(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

void try_convert_localhost(const char** const ip_addr)
{
	assert(ip_addr && "invalid ip addr");
	if (ip_addr == NULL) {
		return;
	}
	if (strcmp(*ip_addr, "localhost") == 0) {
		*ip_addr = "127.0.0.1";
	}
}

/***********************
	unix wrappers
************************/
void* lk_malloc(size_t size)
{
	void* ptr = NULL;
	if ((ptr = malloc(size)) == NULL) {
		err_sys("malloc error");
	}
	return ptr;
}

void* lk_calloc(size_t n, size_t size)
{
	void* ptr = NULL;
	if ((ptr = calloc(n, size)) == NULL) {
		err_sys("calloc error");
	}
	return ptr;
}

void lk_dup2(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1) {
		err_sys("dup2 error");
	}
}

int lk_fcntl(int fd, int cmd, int arg)
{
	int n = -1;
	if ((n = fcntl(fd, cmd, arg)) == -1) {
		err_sys("fcntl error");
	}
	return n;
}

int lk_ioctl(int fd, int request, void* arg)
{
	int n;
	if ((n = ioctl(fd, request, arg)) == -1) {
		err_sys("ioctl error");
	}
	return n; /* streamio of I_LIST returns value */
}

void lk_gettimeofday(struct timeval* tv, void* foo)
{
	if (gettimeofday(tv, foo) == -1) {
		err_sys("gettimeofday error");
	}
}

pid_t lk_fork(void)
{
	pid_t pid;
	if ((pid = fork()) == -1) {
		err_sys("fork error");
	}
	return pid;
}

int lk_open(const char* pathname, int oflag, mode_t mode)
{
	int fd = -1;
	if ((fd = open(pathname, oflag, mode)) == -1) {
		err_sys("open error for %s", pathname);
	}
	return fd;
}

void lk_close(int fd)
{
	if (close(fd) == -1) {
		err_sys("close error");
	}
}

char* lk_strdup(const char* str)
{
	char* ptr = NULL;
	if ((ptr = strdup(str)) == NULL) {
		err_sys("strdup error");
	}
	return ptr;
}

long lk_sysconf(int name)
{
	long val = -1;
	errno = 0; /* in case sysconf() does not change this */
	if ((val = sysconf(name)) == -1) {
		err_sys("sysconf error");
	}
	return val;
}

void lk_unlink(const char* pathname)
{
	if (unlink(pathname) == -1) {
		err_sys("unlink error for %s", pathname);
	}
}

pid_t lk_ait(int* iptr)
{
	pid_t pid = -1;
	if ((pid = wait(iptr)) == -1) {
		err_sys("wait error");
	}
	return pid;
}

pid_t lk_aitpid(pid_t pid, int* iptr, int options)
{
	pid_t retpid = -1;
	if ((retpid = waitpid(pid, iptr, options)) == -1) {
		err_sys("waitpid error");
	}
	return retpid;
}

void lk_write(int fd, void* ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes) {
		err_sys("write error");
	}
}

/***********************
	socket wrappers
************************/
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
