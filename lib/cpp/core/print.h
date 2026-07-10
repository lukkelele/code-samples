#pragma once

#include <chrono>
#include <cstddef>
#include <format>
#include <print>
#include <string>
#include <string_view>

namespace core {
	namespace ansi {
		inline constexpr std::string_view RESET = "\033[0m";
		inline constexpr std::string_view RED = "\033[31m";
		inline constexpr std::string_view GREEN = "\033[32m";
		inline constexpr std::string_view YELLOW = "\033[33m";
		inline constexpr std::string_view BLUE = "\033[34m";
		inline constexpr std::string_view MAGENTA = "\033[35m";
		inline constexpr std::string_view CYAN = "\033[36m";
		inline constexpr std::string_view WHITE = "\033[37m";
	}

	namespace profiler {
		void print(std::string_view function, std::string_view message, std::chrono::nanoseconds elapsed);
	}
}
