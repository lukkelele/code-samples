#include "transport.h"

#include "log.h"
#include "ssl.h"

#include <sys/socket.h>
#include <utility>

/***************************************************
 * TCP Transport
 ***************************************************/
TcpTransport::TcpTransport(Socket s)
	: m_sock(std::move(s))
{
}

ssize_t TcpTransport::read(std::span<char> buf)
{
	return ::recv(m_sock.get(), buf.data(), buf.size(), 0);
}

ssize_t TcpTransport::write(std::span<const char> buf)
{
	return ::send(m_sock.get(), buf.data(), buf.size(), 0);
}

/***************************************************
 * TLS Transport
 ***************************************************/
TlsTransport::TlsTransport(Socket sock, void* const ssl_ctx)
	: m_sock(std::move(sock))
{
	SSL* ssl = SSL_new(static_cast<SSL_CTX*>(ssl_ctx));
	if (ssl == nullptr) {
		LOG_INFO("[tls] SSL_new failed");
		return;
	}

	SSL_set_fd(ssl, m_sock.get());

	const int rc = SSL_accept(ssl);
	if (rc <= 0) {
		const int err = SSL_get_error(ssl, rc);
		LOG_INFO("[tls] SSL_accept failed: rc={} err={}", rc, err);
		SSL_free(ssl);
		return;
	}

	m_ssl = ssl;
	LOG_INFO("[tls] handshake complete: {} {}", SSL_get_version(ssl), SSL_get_cipher(ssl));
}

TlsTransport::~TlsTransport()
{
	if (m_ssl != nullptr) {
		SSL* ssl = static_cast<SSL*>(m_ssl);
		SSL_shutdown(ssl);
		SSL_free(ssl);
		m_ssl = nullptr;
	}
}

ssize_t TlsTransport::read(std::span<char> buf)
{
	if (m_ssl == nullptr) {
		return -1;
	}

	SSL* ssl = static_cast<SSL*>(m_ssl);
	const int n = SSL_read(ssl, buf.data(), static_cast<int>(buf.size()));
	if (n > 0) {
		return n;
	}

	if (SSL_get_error(ssl, n) == SSL_ERROR_ZERO_RETURN) {
		return 0;
	}
	return -1;
}

ssize_t TlsTransport::write(std::span<const char> buf)
{
	if (m_ssl == nullptr) {
		return -1;
	}
	SSL* ssl = static_cast<SSL*>(m_ssl);
	const int n = SSL_write(ssl, buf.data(), static_cast<int>(buf.size()));
	return (n > 0) ? n : -1;
}

std::unique_ptr<ITransport> tcp_factory(Socket sock, void* const ctx)
{
	return std::make_unique<TcpTransport>(std::move(sock));
}

std::unique_ptr<ITransport> tls_factory(Socket sock, void* const ctx)
{
	auto* ssl_ctx = static_cast<SslContext*>(ctx);
	return std::make_unique<TlsTransport>(std::move(sock), ssl_ctx->get());
}
