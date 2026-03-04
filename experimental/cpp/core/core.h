#pragma once

#include <array>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <concepts>
#include <mutex>
#include <print>
#include <string_view>
#include <thread>
#include <type_traits>

#define ASSERT(_expr, ...) assert(_expr)

using namespace std::chrono_literals;

namespace core {

	void print_banner(std::string_view text);

	template<typename... Args>
	inline void log(std::format_string<Args...> fmt, Args&&... args)
	{
		std::println("{}", std::format(fmt, std::forward<Args>(args)...).c_str());
	}

	void log_current_thread();

	template<typename T>
	concept Formattable = std::formattable<T, char>;

}
