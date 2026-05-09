#include "ssl.h"

#include <utility>

#include <openssl/err.h>

#include "log.h"

SslContext::~SslContext()
{
	if (m_ctx != nullptr) {
		SSL_CTX_free(static_cast<SSL_CTX*>(m_ctx));
		m_ctx = nullptr;
	}
}

SslContext::SslContext(SslContext&& other) noexcept
	: m_ctx(std::exchange(other.m_ctx, nullptr))
	, m_alpn(std::move(other.m_alpn))
{
}

SslContext& SslContext::operator=(SslContext&& other) noexcept
{
	if (this != &other) {
		if (m_ctx != nullptr) {
			SSL_CTX_free(static_cast<SSL_CTX*>(m_ctx));
		}
		m_ctx = std::exchange(other.m_ctx, nullptr);
		m_alpn = std::move(other.m_alpn);
	}
	return *this;
}

static int alpn_select_cb(SSL*, const unsigned char** out, unsigned char* out_len, const unsigned char* in, unsigned int in_len, void* arg)
{
	const std::string& wanted = *static_cast<const std::string*>(arg);
	if (wanted.empty()) {
		return SSL_TLSEXT_ERR_NOACK;
	}

	for (unsigned int i = 0; i < in_len;) {
		const unsigned int len = in[i];
		if (i + 1 + len > in_len) {
			break;
		}
		const std::string_view candidate(reinterpret_cast<const char*>(in + i + 1), len);
		if (candidate == wanted) {
			*out = in + i + 1;
			*out_len = static_cast<unsigned char>(len);
			return SSL_TLSEXT_ERR_OK;
		}
		i += 1 + len;
	}
	return SSL_TLSEXT_ERR_NOACK;
}

bool SslContext::init(const TlsCredentials& creds)
{
	SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
	if (ctx == nullptr) {
		LOG_INFO("[tls] SSL_CTX_new failed");
		return false;
	}

	SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
	SSL_CTX_set_options(ctx, SSL_OP_NO_COMPRESSION);

	LOG_INFO(R"(cert: "{}")", creds.cert_path);
	if (SSL_CTX_use_certificate_file(ctx, creds.cert_path.c_str(), SSL_FILETYPE_PEM) != 1) {
		LOG_INFO("[tls] failed to load cert: {}", creds.cert_path);
		SSL_CTX_free(ctx);
		return false;
	}

	LOG_INFO(R"(key: "{}")", creds.key_path);
	if (SSL_CTX_use_PrivateKey_file(ctx, creds.key_path.c_str(), SSL_FILETYPE_PEM) != 1) {
		LOG_INFO("[tls] failed to load key: {}", creds.key_path);
		SSL_CTX_free(ctx);
		return false;
	}

	if (SSL_CTX_check_private_key(ctx) != 1) {
		LOG_INFO("[tls] private key does not match certificate");
		SSL_CTX_free(ctx);
		return false;
	}

	if (!creds.ca_path.empty()) {
		if (SSL_CTX_load_verify_locations(ctx, creds.ca_path.c_str(), nullptr) != 1) {
			LOG_INFO("[tls] failed to load CA: {}", creds.ca_path);
			SSL_CTX_free(ctx);
			return false;
		}
	}

	if (creds.require_client_cert) {
		SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
	}

	m_alpn = creds.alpn_protocols;
	if (!m_alpn.empty()) {
		SSL_CTX_set_alpn_select_cb(ctx, alpn_select_cb, &m_alpn);
	}

	m_ctx = ctx;
	LOG_INFO(R"([tls] context ready: cert="{}" key="{}" alpn="{}")", creds.cert_path, creds.key_path, m_alpn);
	return true;
}
