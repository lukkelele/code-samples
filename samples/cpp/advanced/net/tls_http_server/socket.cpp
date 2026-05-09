#include "socket.h"

#include "log.h"

#include <string>

Socket::Socket(Socket&& other) noexcept
	: m_fd(other.release())
{
}

Socket& Socket::operator=(Socket&& other) noexcept
{
	if (this != &other) {
		reset();
		m_fd = other.release();
	}
	return *this;
}

Socket Socket::listen(std::string_view host, std::uint16_t port, int backlog)
{
	const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd == INVALID_FD) {
		LOG_INFO("[socket] socket() failed");
		return {};
	}

	int opt = 1;
	::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	const std::string host_str(host);
	if (::inet_pton(AF_INET, host_str.c_str(), &addr.sin_addr) != 1) {
		LOG_INFO("[socket] invalid host: {}", host);
		::close(fd);
		return {};
	}

	if (::bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0) {
		LOG_INFO("[socket] bind() failed on {}:{}", host, port);
		::close(fd);
		return {};
	}

	if (::listen(fd, backlog) != 0) {
		LOG_INFO("[socket] listen() failed");
		::close(fd);
		return {};
	}

	return Socket{fd};
}
