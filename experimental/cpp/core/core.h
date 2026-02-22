#pragma once

#include <cassert>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <print>
#include <string_view>
#include <type_traits>

#define ASSERT(_expr, ...) assert(_expr)

namespace core {

	void print_banner(std::string_view text);

	template<typename... Args>
	inline void log(std::format_string<Args...> fmt, Args&&... args)
	{
		std::println("{}", std::format(fmt, std::forward<Args>(args)...).c_str());
	}

}
