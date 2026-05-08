#pragma once

#include <cstddef>
#include <cstdint>
#include <chrono>
#include <optional>
#include <span>
#include <string_view>

#include "socket_utils.h"

namespace net {
	class tcp_socket
	{
	public:
		tcp_socket() = default;
		~tcp_socket();
		tcp_socket(tcp_socket&& other) noexcept;
		tcp_socket(const tcp_socket&) = delete;

		tcp_socket& operator=(tcp_socket&& other) noexcept;
		tcp_socket& operator=(const tcp_socket&) = delete;

		/** @brief Bind to (host, port) and start listening. */
		static tcp_socket listen(std::string_view host, std::uint16_t port, int backlog = 4);
		static tcp_socket listen(std::string_view host, std::uint16_t port, std::string name, int backlog = 4);

		/**
		 * @brief Block until a peer connects.
		 * @todo: Does not really make sense to return std::optional here as the function is blocking.
		 */
		std::optional<tcp_socket> accept_one();

		/** @brief Connect to a remote (host, port). */
		static std::optional<tcp_socket> connect(std::string_view host, std::uint16_t port);
		static std::optional<tcp_socket> connect(std::string_view host, std::uint16_t port, std::string name);

		std::size_t send_all(std::span<const std::byte> data);
		std::size_t send_all(std::string_view text);

		/**
		 * @brief Read up to buffer.size() bytes.
		 * Returns 0 if the peer closed cleanly.
		 */
		std::size_t receive(std::span<std::byte> buf, int flags = 0);

		template<typename Rep, typename Period>
		std::optional<std::size_t> try_receive(const std::chrono::duration<Rep, Period> timeout, std::span<std::byte> buf, const int flags = 0)
		{
			const auto us = std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();
			timeval tv{
				.tv_sec = static_cast<long>(us / 1'000'000),
				.tv_usec = static_cast<long>(us % 1'000'000),
			};

			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(m_fd, &fds);
			if (::select(m_fd + 1, &fds, nullptr, nullptr, &tv) <= 0) {
				return std::nullopt;
			}
			return receive(buf, flags);
		}

		[[nodiscard]] int get_fd() const noexcept { return m_fd; }
		[[nodiscard]] bool is_open() const noexcept { return m_fd != INVALID_SOCKET; }
		[[nodiscard]] std::string_view get_name() const { return m_name; }

	private:
		explicit tcp_socket(int handle);

		int m_fd = INVALID_SOCKET;
		std::string m_name;
	};
}
