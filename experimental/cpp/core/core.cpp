#include "core.h"

#include <cstdio>

namespace core {

	void print_banner(std::string_view text)
	{
		static constexpr size_t padding = 2;
		const size_t len = text.length();
		const size_t total_width = len + (padding * 2);

		std::println();
		for (size_t i = 0; i < total_width; i++) {
			std::putchar('=');
		}

		std::printf("\n%*s%.*s\n",
					static_cast<int>(padding), "",
					static_cast<int>(len), text.data());

		for (std::size_t i = 0; i < total_width; ++i) {
			std::putchar('=');
		}

		std::println("\n");
	}

}
