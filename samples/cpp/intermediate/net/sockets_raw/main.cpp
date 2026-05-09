#include <array>
#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <print>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string_view>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

constexpr int INVALID_SOCK = -1;
constexpr int SOCK_ERROR = -1;

constexpr std::string_view HOST_IP_ADDR = "127.0.0.1";
constexpr std::uint16_t PORT = 54321;
constexpr std::string_view MESSAGE = "the client LUKAS says hello :)";

static void close_sock(int s)
{
	::close(s);
}

static sockaddr_in create_addr()
{
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	::inet_pton(AF_INET, HOST_IP_ADDR.data(), &addr.sin_addr);
	return addr;
}

static void run_server()
{
	const int server = ::socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCK) {
		std::println("[server] socket() failed");
		return;
	}

	int opt = 1;
	::setsockopt(server, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

	const sockaddr_in addr = create_addr();
	if (::bind(server, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCK_ERROR) {
		std::println("[server] bind() failed");
		close_sock(server);
		return;
	}

	if (::listen(server, 1) == SOCK_ERROR) {
		std::println("[server] listen() failed");
		close_sock(server);
		return;
	}
	std::println("[server] listening on {}:{}", HOST_IP_ADDR, PORT);

	const int client = ::accept(server, nullptr, nullptr);
	if (client == INVALID_SOCK) {
		std::println("[server] accept() failed");
		close_sock(server);
		return;
	}
	std::println("[server] client connected");

	std::array<char, 256> buffer{};
	const int received = ::recv(client, buffer.data(), static_cast<int>(buffer.size()), 0);
	if (received > 0) {
		std::println(R"([server] received {} bytes: "{}")", received, std::string_view(buffer.data(), received));
	}

	constexpr std::string_view reply = "ack";
	::send(client, reply.data(), static_cast<int>(reply.size()), 0);

	close_sock(client);
	close_sock(server);
}

int main(int argc, char** argv)
{
	std::println("\n==============================");
	std::println("  sample: net: sockets_raw");
	std::println("==============================\n");

	std::jthread server_thread(run_server);
	std::this_thread::sleep_for(100ms);

	const int client = ::socket(AF_INET, SOCK_STREAM, 0);
	if (client == INVALID_SOCK) {
		std::println("[client] socket() failed");
		return 1;
	}

	const sockaddr_in addr = create_addr();
	if (::connect(client, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCK_ERROR) {
		std::println("[client] connect() failed");
		close_sock(client);
		return 1;
	}
	std::println("[client] connected to {}:{}", HOST_IP_ADDR, PORT);

	::send(client, MESSAGE.data(), MESSAGE.size(), 0);
	std::println(R"([client] sent {} bytes: "{}")", MESSAGE.size(), MESSAGE);

	std::array<char, 64> buffer{};
	const ssize_t received = ::recv(client, buffer.data(), static_cast<int>(buffer.size()), 0);
	if (received > 0) {
		std::println(R"([client] reply: "{}")", std::string_view(buffer.data(), received));
	}

	close_sock(client);
	return 0;
}
