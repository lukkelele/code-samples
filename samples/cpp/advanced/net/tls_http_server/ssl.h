#pragma once

#include <string>
#include <sys/types.h>

#include <openssl/ssl.h>

struct TlsCredentials
{
	std::string cert_path;
	std::string key_path;
	std::string ca_path;
	std::string alpn_protocols = "http/1.1";
	bool require_client_cert = false;
};

class SslContext
{
public:
	SslContext() = default;
	~SslContext();
	SslContext(SslContext&& other) noexcept;
	SslContext(const SslContext&) = delete;

	SslContext& operator=(SslContext&& other) noexcept;
	SslContext& operator=(const SslContext&) = delete;

	[[nodiscard]] bool init(const TlsCredentials& creds);
	[[nodiscard]] void* get() const noexcept { return m_ctx; }
	[[nodiscard]] bool valid() const noexcept { return m_ctx != nullptr; }

private:
	void* m_ctx = nullptr;
	std::string m_alpn;
};
