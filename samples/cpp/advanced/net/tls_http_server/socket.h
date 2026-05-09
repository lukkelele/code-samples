#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <string_view>
#include <utility>

class Socket
{
public:
	static constexpr int INVALID_FD = -1;

public:
	Socket() = default;
	explicit Socket(int fd)
		: m_fd(fd)
	{}
	~Socket() { reset(); }

	Socket(Socket&& other) noexcept;
	Socket(const Socket&) = delete;

	Socket& operator=(Socket&& other) noexcept;
	Socket& operator=(const Socket&) = delete;

	[[nodiscard]] int get() const noexcept { return m_fd; }
	[[nodiscard]] bool valid() const noexcept { return m_fd != INVALID_FD; }

	int release() noexcept
	{
		const int fd = m_fd;
		m_fd = INVALID_FD;
		return fd;
	}

	void reset() noexcept
	{
		if (m_fd != INVALID_FD) {
			::close(m_fd);
			m_fd = INVALID_FD;
		}
	}

	[[nodiscard]] static Socket listen(std::string_view host, std::uint16_t port, int backlog = 64);

private:
	int m_fd = INVALID_FD;
};
