#include "socket_utils.h"

#include <cerrno>
#include <cstring>

namespace net {
	void close_socket(int& sock)
	{
		if (sock == INVALID_SOCKET) {
			return;
		}
		::close(sock);
		sock = INVALID_SOCKET;
	}

	std::string last_error()
	{
		return std::strerror(errno);
	}
}
