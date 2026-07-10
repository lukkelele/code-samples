#include "print.h"

namespace core {
	namespace profiler {
		static constexpr bool COLORED = true;
		static constexpr std::size_t FUNC_WIDTH = 24;
		static constexpr std::size_t MSG_WIDTH = 22;
		static constexpr std::string_view PERFECT_PERFORMANCE_COLOR = ansi::GREEN;
		static constexpr std::string_view GOOD_PERFORMANCE_COLOR = ansi::WHITE;
		static constexpr std::string_view WORRYSOME_PERFORMANCE_COLOR = ansi::YELLOW;
		static constexpr std::string_view BAD_PERFORMANCE_COLOR = ansi::RED;

		static void set_line_format(double& time, const bool time_microseconds, std::string_view& unit, std::string_view& color)
		{
			if (time_microseconds) {
				unit = "us";
				if (time <= 5) {
					color = PERFECT_PERFORMANCE_COLOR;
				} else {
					color = GOOD_PERFORMANCE_COLOR;
				}
			} else {
				if (time >= 1000) {
					unit = "s";
					color = BAD_PERFORMANCE_COLOR;
					time /= 1000.0;
				} else if ((time >= 500)) {
					unit = "ms";
					color = BAD_PERFORMANCE_COLOR;
				} else {
					unit = "ms";
					color = WORRYSOME_PERFORMANCE_COLOR;
				}
			}
		}
	}

	void profiler::print(const std::string_view function, const std::string_view message, const std::chrono::nanoseconds elapsed)
	{
		const auto ns = elapsed.count();
		const bool micros = ns < 1'000'000;
		double value = micros ? (static_cast<double>(ns) / 1000.0) : (static_cast<double>(ns) / 1'000'000.0);
		std::string_view unit;
		std::string_view color;
		set_line_format(value, micros, unit, color);

		// const std::string_view color = micros ? cfg.micro_color : cfg.milli_color;
		// const std::string function_col = std::format("[{:<{}}]", function, cfg.function_width);
		const std::string func_col = std::format("[{:<{}}]", function, FUNC_WIDTH);
		// const std::string message_col = message.empty() ? std::string(static_cast<std::size_t>(cfg.message_width) + 2, ' ') : std::format("[{:<{}}]", message, cfg.message_width);
		// const std::string msg_col = message.empty() ? std::string(MSG_WIDTH + 2, ' ') : std::format("[{:<{}}]", message, MSG_WIDTH);
		// const std::string msg_col = message.empty() ? "" : std::format("[{:<{}}]", message, MSG_WIDTH);
		// const std::string msg_col = message.empty() ? "" : std::format("| {}", message);
		const std::string msg_col = message.empty() ? "" : std::format("|  {}", message);

		// const std::string time_col = std::format("[{:>8.3f} {}]", value, unit);
		// const std::string time_col = std::format("[{:>8.3f} {:<{}}]", value, unit, time_unit_len);
		// const std::string time_col = std::format("[{:>8.3f} {:>{}}]", value, unit, time_unit_len);
		// const std::string time_col = std::format("[{:>{}.3f} {}]", value, time_unit_len, unit);
		const std::string time_col = std::format("[{:>8.3f} {:>2}]", value, unit);
		if (COLORED) {
			// std::println("{}   {}   {}{}{}", func_col, msg_col, color, time_col, ansi::RESET);
			// std::println("{}   {}{}{}   {}", func_col, color, time_col, ansi::RESET, msg_col);
			std::println("{}  {}{}{}  {}", func_col, color, time_col, ansi::RESET, msg_col);
		} else {
			std::println("{}  {}  {}", func_col, msg_col, time_col);
		}
	}
}
