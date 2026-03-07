#pragma once

#include <atomic>

namespace ui {
	/* @fixme: Should be an enum instead, but just trying out for now */
	inline constexpr std::size_t VIEW_DIAL = 0;
	inline constexpr std::size_t VIEW_CONTACTS = 1;

	inline std::atomic<uint8_t> current_view;
	inline std::atomic<uint8_t> next_view;
}
