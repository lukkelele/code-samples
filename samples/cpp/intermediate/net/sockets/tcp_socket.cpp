#include "tcp_socket.h"

#include <format>
#include <stdexcept>
#include <string>
#include <utility>

#include "core/core.h"
#include "socket_utils.h"

namespace net {
	/**
	 * @brief Build an IPv4 sockaddr from a dotted-decimal host and a port.
	 */
	static sockaddr_in make_address(std::string_view host, std::uint16_t port)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		/* inet_pton needs a null-terminated string. */
		const std::string host_str(host);
		if (::inet_pton(AF_INET, host_str.c_str(), &addr.sin_addr) != 1) {
			throw std::runtime_error(std::format("invalid address: {}", host_str));
		}
		std::println("[host] {}:{}", host, port);
		std::println("[host] hex:  {}", core::to_hex(addr));
		std::println("[host] bits: {}", core::to_bits(addr));
		return addr;
	}

	tcp_socket::tcp_socket(const int handle)
		: m_fd(handle)
	{
	}

	tcp_socket::~tcp_socket()
	{
#ifdef LOG_DESTRUCTOR
		std::println(R"([tcp_socket] release: fd={} name="{}")", m_fd, m_name);
#endif
		close_socket(m_fd);
	}

	tcp_socket::tcp_socket(tcp_socket&& other) noexcept
		: m_fd(std::exchange(other.m_fd, INVALID_SOCKET))
		, m_name(std::move(other.m_name))
	{
	}

	tcp_socket& tcp_socket::operator=(tcp_socket&& other) noexcept
	{
		if (this != &other) {
			close_socket(m_fd);
			m_fd = std::exchange(other.m_fd, INVALID_SOCKET);
			m_name = std::move(other.m_name);
		}
		return *this;
	}

	tcp_socket tcp_socket::listen(std::string_view host, std::uint16_t port, const int backlog)
	{
		int sock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			throw std::runtime_error(std::format("socket() failed: {}", last_error()));
		}

		/* Avoid TIME_WAIT issues when re-running the sample. */
		int opt = 1;
		::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

		const sockaddr_in addr = make_address(host, port);
		if (::bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0) {
			const std::string err = last_error();
			close_socket(sock);
			throw std::runtime_error(std::format("bind() failed: {}", err));
		}

		if (::listen(sock, backlog) != 0) {
			const std::string err = last_error();
			close_socket(sock);
			throw std::runtime_error(std::format("listen() failed: {}", err));
		}

		return tcp_socket(sock);
	}

	tcp_socket tcp_socket::listen(std::string_view host, const std::uint16_t port, std::string name, const int backlog)
	{
		auto sock = listen(host, port, backlog);
		sock.m_name = std::move(name);
		return sock;
	}

	std::optional<tcp_socket> tcp_socket::accept_one()
	{
		sockaddr_in peer{};
		socklen_t len = sizeof(peer);
		const int client = ::accept(m_fd, reinterpret_cast<sockaddr*>(&peer), &len);
		if (client == INVALID_SOCKET) {
			std::println("accept() failed: {}", last_error());
			return std::nullopt;
		}

		std::optional<tcp_socket> opt = {tcp_socket(client)};
		if (!m_name.empty()) {
			opt.value().m_name = m_name;
		}
		return opt;
	}

	std::optional<tcp_socket> tcp_socket::connect(std::string_view host, std::uint16_t port)
	{
		int s = ::socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET) {
			std::println("socket() failed: {}", last_error());
			return std::nullopt;
		}

		const sockaddr_in addr = make_address(host, port);
		if (::connect(s, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0) {
			const std::string err = last_error();
			close_socket(s);
			std::println("connect() failed: {}", err);
			return std::nullopt;
		}
		return {tcp_socket(s)};
	}

	std::optional<tcp_socket> tcp_socket::connect(std::string_view host, std::uint16_t port, std::string name)
	{
		auto opt = connect(host, port);
		if (opt.has_value()) {
			opt.value().m_name = std::move(name);
		}
		return {std::move(opt.value())};
	}

	std::size_t tcp_socket::send_all(std::span<const std::byte> data)
	{
		std::size_t total = 0;
		while (total < data.size()) {
			const std::size_t remaining = data.size() - total;
			const ssize_t sent = ::send(m_fd, reinterpret_cast<const char*>(data.data() + total), remaining, 0);
			if (sent <= 0) {
				throw std::runtime_error(std::format("send() failed: {}", last_error()));
			}

			total += static_cast<std::size_t>(sent);
		}
		return total;
	}

	std::size_t tcp_socket::send_all(std::string_view text)
	{
		const std::byte* bytes = reinterpret_cast<const std::byte*>(text.data());
		return send_all(std::span<const std::byte>(bytes, text.size()));
	}

	std::size_t tcp_socket::receive(std::span<std::byte> buf, const int flags)
	{
		if (m_fd == INVALID_SOCKET) {
			std::println("[tcp_socket] Cannot receive data, socket is invalid");
			return 0;
		}
#ifdef LOG_RECEIVE
		if (m_name.empty()) {
			std::println("[tcp_socket::receive] handle={} flags={}", m_fd, flags);
		} else {
			std::println("[tcp_socket::receive] [{}] handle={} flags={}", m_name, m_fd, flags);
		}
#endif
		const ssize_t received = ::recv(
			m_fd,
			reinterpret_cast<char*>(buf.data()),
			static_cast<int>(buf.size()),
			flags);
		return static_cast<std::size_t>(received);
	}
}
