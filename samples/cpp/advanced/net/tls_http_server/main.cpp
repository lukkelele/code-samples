#include <print>
#include <string_view>

#include "log.h"
#include "router.h"
#include "server.h"
#include "ssl.h"
#include "transport.h"
#include "users.h"

constexpr std::string_view HOST = "0.0.0.0";
constexpr std::uint16_t PORT = 8080;

constexpr bool USE_TLS = true;

int main(int argc, char** argv)
{
	std::println("\n==============================");
	std::println("  sample: net: tls_http_server");
	std::println("==============================\n");

	LOG_INFO("[main] routes:");
	LOG_INFO("  GET    /users");
	LOG_INFO("  POST   /users        body: {{\"name\":\"...\",\"email\":\"...\"}}");
	LOG_INFO("  GET    /users/:id");
	LOG_INFO("  DELETE /users/:id");
	LOG_INFO("");

	UserStore store;
	Router r;
	register_user_routes(r, store);

	SslContext ssl_ctx;
	TransportFactory factory = tcp_factory;
	void* factory_ctx = nullptr;

	if constexpr (USE_TLS) {
		const TlsCredentials creds{
			.cert_path = SAMPLE_DIR "/certs/server.crt",
			.key_path = SAMPLE_DIR "/certs/server.key",
			.ca_path = SAMPLE_DIR "/certs/ca.crt",
			.alpn_protocols = "http/1.1",
			.require_client_cert = false,
		};
		if (!ssl_ctx.init(creds)) {
			LOG_INFO("[main] TLS init failed");
			return 1;
		}

		factory = tls_factory;
		factory_ctx = &ssl_ctx;
	}

	HttpServer server{r, factory, factory_ctx};
	if (!server.listen(HOST, PORT)) {
		return 1;
	}

	server.run();
	return 0;
}
