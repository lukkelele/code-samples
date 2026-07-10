#include "core.h"

#include <cstdio>
#include <thread>

namespace core {
	void print_banner(std::string_view text, const std::size_t banner_len, const char banner_symbol, const std::size_t padding)
	{
		const std::size_t len = text.length();
		const std::size_t calc_banner_len = len + (2 * padding);
		const std::size_t total_width = (calc_banner_len > banner_len) ? calc_banner_len : banner_len;

		std::println();
		for (std::size_t i = 0; i < total_width; i++) {
			std::putchar(banner_symbol);
		}

		std::printf("\n%*s%.*s\n", static_cast<int>(padding), "", static_cast<int>(len), text.data());
		for (std::size_t i = 0; i < total_width; i++) {
			std::putchar(banner_symbol);
		}

		std::println("\n");
	}

	void log_current_thread()
	{
		log("thread: {}", std::this_thread::get_id());
	}

	std::string bytes_to_hex(std::span<const unsigned char> bytes)
	{
		static constexpr char HEX_CHARS[] = "0123456789abcdef";
		std::string result;
		result.reserve(bytes.size() * 3);

		for (std::size_t i = 0; i < bytes.size(); ++i) {
			const unsigned char byte = bytes[i];
			result.push_back(HEX_CHARS[(byte >> 4) & 0x0F]);
			result.push_back(HEX_CHARS[byte & 0x0F]);
			if ((i + 1) < bytes.size()) {
				result.push_back(' ');
			}
		}

		return result;
	}

	std::string bytes_to_bits(std::span<const unsigned char> bytes)
	{
		std::string result;
		result.reserve(bytes.size() * 9);

		for (std::size_t byte_idx = 0; byte_idx < bytes.size(); byte_idx++) {
			const unsigned char byte = bytes[byte_idx];
			for (int bit_idx = 7; bit_idx >= 0; bit_idx--) {
				result.push_back(((byte >> bit_idx) & 1) != 0 ? '1' : '0');
			}

			if ((byte_idx + 1) < bytes.size()) {
				result.push_back(' ');
			}
		}

		return result;
	}

	std::span<const unsigned char> as_bytes(const sockaddr_in& addr)
	{
		const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&addr);
		return {bytes, sizeof(addr)};
	}

	std::string to_hex(const sockaddr_in& addr)
	{
		return bytes_to_hex(as_bytes(addr));
	}

	std::string to_bits(const sockaddr_in& addr)
	{
		return bytes_to_bits(as_bytes(addr));
	}
}
