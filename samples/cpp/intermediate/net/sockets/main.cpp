#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string_view>
#include <thread>

#include "socket_utils.h"
#include "tcp_socket.h"

using namespace std::chrono_literals;

constexpr std::string_view HOST_IP_ADDR = "127.0.0.1";
constexpr std::uint16_t PORT = 54321;
std::atomic_bool server_thread_running = true;

static void run_server()
{
	using namespace net;
	tcp_socket listener = tcp_socket::listen(HOST_IP_ADDR, PORT, "server");
	std::println("[server] listening on {}:{}", HOST_IP_ADDR, PORT);

	std::optional<tcp_socket> client_opt = listener.accept_one();
	if (!client_opt.has_value()) {
		std::exit(EXIT_FAILURE);
	}
	tcp_socket client = std::move(client_opt.value());
	std::println("[server] client connected (fd={} name=\"{}\")\n", client.get_fd(), client.get_name());

	std::array<std::byte, 256> buffer{};
	while (server_thread_running) {
		const auto received = client.try_receive(10ms, buffer);
		if (!received.has_value()) {
			continue;
		}

		const std::string_view text(reinterpret_cast<const char*>(buffer.data()), received.value());
		std::println(R"([server] received {} bytes: "{}")", received.value(), text);
		std::fflush(stdout);

		client.send_all("ACK");
		buffer.fill(std::byte{0});
	}
}

int main()
{
	std::println("\n==============================");
	std::println("  sample: net: sockets");
	std::println("==============================\n");
	using namespace net;

	std::jthread server_thread(run_server);
	/* Pause so the listener has time to bind before we connect. */
	std::this_thread::sleep_for(50ms);

	std::optional<tcp_socket> client_opt = tcp_socket::connect(HOST_IP_ADDR, PORT, "main");
	if (!client_opt.has_value()) {
		std::exit(EXIT_FAILURE);
	}
	tcp_socket client = std::move(client_opt.value());
	std::println("[client] [{}] connected to {}:{}", client.get_name(), HOST_IP_ADDR, PORT);
	std::this_thread::sleep_for(300ms);

	constexpr std::string_view MESSAGE_1 = "hejsan svejsan from first transmission";
	client.send_all(MESSAGE_1);
	std::println(R"([client] sent {} bytes -> "{}", waiting for ACK)", MESSAGE_1.size(), MESSAGE_1);
	{
		std::array<std::byte, 64> buffer{};
		const std::size_t received = client.receive(buffer);
		const std::string_view reply(reinterpret_cast<const char*>(buffer.data()), received);
		std::println(R"([client] [{}] reply 1: "{}")", client.get_name(), reply);
	}

	std::println("\n==========================\n");
	std::fflush(stdout);

	constexpr std::string_view MESSAGE_2 = "second transmission";
	client.send_all(MESSAGE_2);
	std::println(R"([client] sent {} bytes -> "{}", waiting for ACK)", MESSAGE_2.size(), MESSAGE_2);
	{
		std::array<std::byte, 64> buffer{};
		const std::size_t received = client.receive(buffer);
		const std::string_view reply(reinterpret_cast<const char*>(buffer.data()), received);
		std::println(R"([client] [{}] reply 2: "{}")", client.get_name(), reply);
	}

	server_thread_running = false;
	server_thread.join();
	std::println("Exit main");
	return 0;
}
