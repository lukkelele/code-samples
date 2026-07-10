#pragma once

#include <arpa/inet.h>
#include <array>
#include <cassert>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <print>
#include <span>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>

#define ASSERT(_expr, ...) assert(_expr)

using namespace std::chrono_literals;

namespace core {
	void print_banner(std::string_view text, std::size_t banner_len = 0, char banner_symbol = '=', std::size_t padding = 4);

	template<typename... Args>
	inline void log(std::format_string<Args...> fmt, Args&&... args)
	{
		std::println("{}", std::format(fmt, std::forward<Args>(args)...).c_str());
	}

	void log_current_thread();

	template<typename T>
	concept Formattable = std::formattable<T, char>;

	std::string bytes_to_hex(std::span<const unsigned char> bytes);
	std::string bytes_to_bits(std::span<const unsigned char> bytes);
	std::span<const unsigned char> as_bytes(const sockaddr_in& addr);
	std::string to_hex(const sockaddr_in& addr);
	std::string to_bits(const sockaddr_in& addr);
}
