#ifndef _NGX_SOCKET_H_
#define _NGX_SOCKET_H_

#include <netinet/in.h>
#include <sys/socket.h>

typedef int ngx_socket_t;

#define ngx_socket 			socket
#define ngx_close_socket	close

#define ngx_nonblocking(s)  fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK)
#define ngx_blocking(s)     fcntl(s, F_SETFL, fcntl(s, F_GETFL) & ~O_NONBLOCK)
#define ngx_cloexec(s)	  fcntl(s, F_SETFL, fcntl(s, F_GETFL) & FD_CLOSEXEC)


int ngx_socket_keepalive(ngx_socket_t s);
int ngx_socket_reuseaddr(ngx_socket_t s);
int ngx_socket_timedout(ngx_socket_t s, int timedout);
int ngx_connect_peer(ngx_socket_t s, struct sockaddr *sa, int socklen);
ssize_t ngx_recv(ngx_socket_t s, u_char *buf, size_t size);
ssize_t ngx_send(ngx_socket_t s, u_char *buf, size_t size);
ssize_t ngx_peek(ngx_socket_t s, u_char *buf, size_t size);

#endif