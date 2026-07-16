#ifndef _C_NET
#define _C_NET

#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define LISTEN_QUEUE_SIZE 1024 /* 2nd argument to listen() */
#define MAXLINE           4096 /* max text line length */
#define MAXSOCKADDR       128 /* max socket address structure size */
#define BUFSIZE           8192 /* buffer size for reads and writes */
#define SERV_PORT         9877 /* TCP and UDP client-servers */
#define SERV_PORT_STR     "9877" /* TCP and UDP client-servers */
#define UNIXSTR_PATH      "/tmp/unix.str" /* unix domain stream cli-serv */
#define UNIXDG_PATH       "/tmp/unix.dg" /* unix domain datagram cli-serv */

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) /* default file access permissions for new files */
#define DIR_MODE  (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH) /* default permissions for new directories */

#define LOG(...) log_msg(__VA_ARGS__)

typedef void signal_handler_t(int);

int connect_nonb(int, const struct sockaddr*, socklen_t, int);
int connect_timeo(int, const struct sockaddr*, socklen_t, int);
void daemon_init(const char*, int);
void daemon_inetd(const char*, int);
void dg_cli(FILE*, int, const struct sockaddr*, socklen_t);
void dg_echo(int, struct sockaddr*, socklen_t);
int family_to_level(int);
char* gf_time(void);
void heartbeat_cli(int, int, int);
void heartbeat_serv(int, int, int);
struct addrinfo* host_serv(const char*, const char*, int, int);
int inet_srcrt_add(char*, int);
u_char* inet_srcrt_init(void);
void inet_srcrt_print(u_char*, int);
char** my_addrs(int*);
int readable_timeo(int, int);

ssize_t readline(int, void*, size_t);
ssize_t readn(int, void*, size_t);
ssize_t read_fd(int, void*, size_t, int*);
ssize_t recvfrom_flags(int, void*, size_t, int*, struct sockaddr*, socklen_t*, struct in_pktinfo*);
signal_handler_t* signal_intr(int, signal_handler_t*);

int sock_bind_wild(int, int);
int sock_cmp_addr(const struct sockaddr*, const struct sockaddr*, socklen_t);
int sock_cmp_port(const struct sockaddr*, const struct sockaddr*, socklen_t);
int sock_get_port(const struct sockaddr*, socklen_t);
void sock_set_addr(struct sockaddr*, socklen_t, const void*);
void sock_set_port(struct sockaddr*, socklen_t, int);
void sock_set_wild(struct sockaddr*, socklen_t);
char* sock_ntop(const struct sockaddr*, socklen_t);
char* sock_ntop_host(const struct sockaddr*, socklen_t);
int sockfd_to_family(int);

void str_echo(int);
void str_cli(FILE*, int);
int tcp_connect(const char*, const char*);
int tcp_listen(const char*, const char*, socklen_t*);
void tv_sub(struct timeval*, struct timeval*);
int udp_client(const char*, const char*, void**, socklen_t*);
int udp_connect(const char*, const char*);
int udp_server(const char*, const char*, socklen_t*);
int writable_timeo(int, int);
ssize_t writen(int, const void*, size_t);
ssize_t write_fd(int, void*, size_t, int);

/**
 * Convert ip address to '127.0.0.1' if the passed ip is 'localhost' else do nothing.
 */
void try_convert_localhost(const char** ip_addr);

void log_msg(const char*, ...);
void err_quit(const char*, ...);
void err_ret(const char*, ...);
void err_sys(const char*, ...);
void err_dump(const char*, ...);
void err_msg(const char*, ...);

/***********************
 * unix wrappers
 ************************/
void* lk_malloc(size_t size);
void* lk_calloc(size_t n, size_t size);
void lk_dup2(int fd1, int fd2);
int lk_fcntl(int fd, int cmd, int arg);
int lk_ioctl(int fd, int request, void* arg);
void lk_gettimeofday(struct timeval* tv, void* foo);
pid_t lk_fork(void);
int lk_open(const char* pathname, int oflag, mode_t mode);
void lk_close(int fd);
char* lk_strdup(const char* str);
long lk_sysconf(int name);
void lk_unlink(const char* pathname);
pid_t lk_ait(int* iptr);
pid_t lk_aitpid(pid_t pid, int* iptr, int options);
void lk_write(int fd, void* ptr, size_t nbytes);

/***********************
 * socket wrappers
 ************************/
void lk_listen(int fd, int backlog);
int lk_poll(struct pollfd* fds, unsigned long nfds, int timeout);
int lk_socket(int family, int type, int protocol);
void lk_socketpair(int family, int type, int protocol, int* fd);
int lk_accept(int fd, struct sockaddr* sa, socklen_t* sa_len_ptr);
void lk_bind(int fd, const struct sockaddr* sa, socklen_t sa_len);
void lk_connect(int fd, const struct sockaddr* sa, socklen_t sa_len);
ssize_t lk_recv(int fd, void* ptr, size_t nbytes, int flags);
ssize_t lk_recvfrom(int fd, void* ptr, size_t nbytes, int flags, struct sockaddr* sa, socklen_t* sa_len_ptr);
ssize_t lk_recvmsg(int fd, struct msghdr* msg, int flags);
int lk_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);
void lk_send(int fd, const void* data, size_t nbytes, int flags);
void lk_sendto(int fd, const void* data, size_t nbytes, int flags, const struct sockaddr* sa, socklen_t sa_len);
void lk_sendmsg(int fd, const struct msghdr* msg, int flags);
void lk_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen);
void lk_shutdown(int fd, int how);

#endif /* _C_NET */
