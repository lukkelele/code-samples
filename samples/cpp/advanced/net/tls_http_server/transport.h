#pragma once

#include "socket.h"

#include <memory>
#include <span>
#include <string>
#include <sys/types.h>

class ITransport
{
public:
	virtual ~ITransport() = default;
	virtual ssize_t read(std::span<char> buf) = 0;
	virtual ssize_t write(std::span<const char> buf) = 0;
	virtual int fd() const = 0;
};

class TcpTransport final : public ITransport
{
public:
	explicit TcpTransport(Socket s);

	ssize_t read(std::span<char> buf) override;
	ssize_t write(std::span<const char> buf) override;
	int fd() const override { return m_sock.get(); }

private:
	Socket m_sock;
};

class TlsTransport final : public ITransport
{
public:
	TlsTransport(Socket s, void* ssl_ctx);
	~TlsTransport() override;

	ssize_t read(std::span<char> buf) override;
	ssize_t write(std::span<const char> buf) override;
	int fd() const override { return m_sock.get(); }

	bool valid() const noexcept { return m_ssl != nullptr; }

private:
	Socket m_sock;
	void* m_ssl = nullptr;
};

using TransportFactory = std::unique_ptr<ITransport> (*)(Socket, void*);

std::unique_ptr<ITransport> tcp_factory(Socket sock, void* ctx);
std::unique_ptr<ITransport> tls_factory(Socket sock, void* ctx);
