#include "server.h"

#include <array>
#include <charconv>
#include <format>
#include <string>
#include <thread>
#include <utility>
#include <variant>

#include "log.h"

constexpr std::size_t READ_BUFSIZE = 8192;
constexpr std::size_t MAX_REQUEST = 1 << 20;

bool HttpServer::listen(std::string_view host, std::uint16_t port)
{
	m_listener = Socket::listen(host, port);
	if (!m_listener.valid()) {
		return false;
	}
	LOG_INFO("[server] listening on {}:{}", host, port);
	return true;
}

void HttpServer::run()
{
	m_running = true;
	while (m_running) {
		sockaddr_in peer{};
		socklen_t peer_len = sizeof(peer);
		const int client_fd = ::accept(m_listener.get(), reinterpret_cast<sockaddr*>(&peer), &peer_len);
		if (client_fd == Socket::INVALID_FD) {
			continue;
		}

		char buf[INET_ADDRSTRLEN]{};
		::inet_ntop(AF_INET, &peer.sin_addr, buf, sizeof(buf));
		std::string peer_addr = std::format("{}:{}", buf, ntohs(peer.sin_port));

		auto t = m_factory(Socket{client_fd}, m_factory_ctx);

		std::thread([this, t = std::move(t), peer_addr = std::move(peer_addr)]() mutable
		{
			LOG_INFO("[server] connection from {}", peer_addr);
			handle_connection(std::move(t), peer_addr);
			LOG_INFO("[server] closed connection from {}", peer_addr);
		}).detach();
	}
}

void HttpServer::stop() noexcept
{
	m_running = false;
}

static std::size_t expected_body_size(const HttpRequest& req)
{
	const auto it = req.headers.find("content-length");
	if (it == req.headers.end()) {
		return 0;
	}
	std::size_t v = 0;
	std::from_chars(it->second.data(), it->second.data() + it->second.size(), v);
	return v;
}

void HttpServer::handle_connection(std::unique_ptr<ITransport> transport, std::string peer_addr) const
{
	std::string buf;
	buf.reserve(READ_BUFSIZE);
	std::array<char, READ_BUFSIZE> chunk{};

	while (true) {
		const auto header_end = buf.find("\r\n\r\n");
		if (header_end == std::string::npos) {
			const ssize_t n = transport->read(chunk);
			if (n <= 0) {
				break;
			}
			buf.append(chunk.data(), static_cast<std::size_t>(n));
			if (buf.size() > MAX_REQUEST) {
				break;
			}
			continue;
		}

		HttpRequest req;
		if (!parse_request(buf, req)) {
			HttpResponseType br = JSON::bad_request();
			const std::string raw = std::visit([](const auto& r)
			{
				return r.serialize();
			}, br);
			transport->write({raw.data(), raw.size()});
			break;
		}

		const std::size_t expected = expected_body_size(req);
		const std::size_t total_needed = header_end + 4 + expected;
		if (buf.size() < total_needed) {
			const ssize_t n = transport->read(chunk);
			if (n <= 0) {
				break;
			}

			buf.append(chunk.data(), static_cast<std::size_t>(n));
			if (buf.size() > MAX_REQUEST) {
				break;
			}
			continue;
		}

		LOG_INFO("[{}] {} {} {}", peer_addr, method_name(req.method), req.path, req.version);

		HttpResponseType resp = m_router.dispatch(req);
		const bool should_keep_alive = keep_alive(req);

		auto [raw, status] = std::visit([&](auto& r) -> std::pair<std::string, int>
		{
			r.set_header("Connection", should_keep_alive ? "keep-alive" : "close");
			r.set_header("Server", "advanced-net-http/1.0");
			return {r.serialize(), r.status};
		}, resp);

		transport->write({raw.data(), raw.size()});
		LOG_INFO("[{}] -> {}", peer_addr, status);

		if (!should_keep_alive) {
			break;
		}

		buf.erase(0, total_needed);
	}
}
