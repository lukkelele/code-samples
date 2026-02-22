#pragma once

#include <cassert>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <string_view>
#include <type_traits>

#define ASSERT(_expr, ...) assert(_expr)

namespace core {

	void print_banner(std::string_view text);
	void log(std::string_view msg);

}
