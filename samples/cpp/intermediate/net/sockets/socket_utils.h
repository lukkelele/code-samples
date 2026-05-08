#pragma once

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

namespace net {
	inline constexpr int INVALID_SOCKET = -1;

	void close_socket(int& sock);
	std::string last_error();
}
