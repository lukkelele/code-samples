#include "net.h"

#include <assert.h>
#include <stdarg.h>
#include <syslog.h>

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
