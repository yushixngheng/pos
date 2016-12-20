#include "ngx_core.h"

int ngx_connect_peer(ngx_socket_t s, struct sockaddr *sa, int socklen) {
	int rv, err;

	rv = connect(s, sa, socklen);
	if (rv != 0) {
		err =errno;
		if (err == EINPROGRESS) {
			return 0;
		}
		printf("connect error %d\n", err);
		return -1;
	}
	return 0;
}


ssize_t ngx_recv(ngx_socket_t s, u_char *buf, size_t size) {
    ssize_t       n;
    int     err;
    do {
        n = recv(s, buf, size, 0);
        if (n == 0) {
            return n;
        } else if (n > 0) {
        	return n;
        }
        err = errno;
        if (err == EAGAIN || err == EINTR) {  //如果是INTR的话继续接收
            if (err == EINTR) {
            }
            n = -2;
        } else {    //发生了其它错误，退出
            printf("recv error %d\n", err);
            break;
        }
    } while (err == EINTR);
    return n;
}

ssize_t ngx_send(ngx_socket_t s, u_char *buf, size_t size) {
    ssize_t       n;
    int     err;

    for ( ;; ) {
        n = send(s, buf, size, 0);
        if (n > 0) {
            if (n <= (ssize_t) size) {
            }
            return n;
        }
        err = errno;
        if (n == 0) {
            return n;
        }
        if (err == EAGAIN || err == EINTR) {        //如果是INTR的话继续发送
            if (err == EAGAIN) {
                return -2;
            }

        } else {
            printf("send error %d\n", err);
            return -1;
        }
    }
}

ssize_t
ngx_peek(ngx_socket_t s, u_char *buf, size_t size)
{
    ssize_t       n;
    int     err;
    do {
        n = recv(s, buf, size, MSG_PEEK);
        if (n == 0) {
            return n;
        } else if (n > 0) {
            return n;
        }
        err = errno;
        if (err == EAGAIN || err == EINTR) {  //如果是INTR的话继续接收
            if (err == EINTR) {
                ErrorLog(ERROR, "peek interrupt by INTR");
            }
            else {
                n = NGX_AGAIN;
                ErrorLog(ERROR, "peek return EAGAIN");

            }
        } else {    //发生了其它错误，退出
            ErrorLog(ERROR, "peek error %d", err);
            break;
        }
    } while (err == EINTR);
    return n;
}


int
ngx_socket_keepalive(ngx_socket_t s)
{
	int flag;
	int err;

	flag = 1;
	if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) == -1) {
		err = errno;
		printf("setsockopt keepalive error [%d]", err);
		return -1;
	}
	return 0;
}


int
ngx_socket_reuseaddr(ngx_socket_t s)
{
	int flag;
	int err;

	flag = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
		err = errno;
		printf("setsockopt reuseaddr error [%d]", err);
		return -1;
	}
	return 0;
}


int
ngx_socket_timedout(ngx_socket_t s, int timedout)
{
	struct timeval to;
	int err;

	if (timedout >= 1000) {
		to.tv_sec = timedout / 1000;
		to.tv_usec = (timedout % 1000) * 1000;
	}
	else {
		to.tv_sec = 0;
		to.tv_usec = timedout * 1000;
	}
	if (setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,(const char*)&to,sizeof(to)) == -1) {
		err = errno;
		printf("setsockopt send timedout [%d]\n", err);
		return -1;
	}
	if (setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(const char*)&to,sizeof(to)) == -1) {
		err = errno;
		printf("setsockopt recv timedout [%d]\n", err);
		return -1;
	}
	return 0;
}

