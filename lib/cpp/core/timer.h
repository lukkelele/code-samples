#pragma once

#include <chrono>
#include <cstdint>
#include <string_view>

#include "print.h"

using namespace std::chrono_literals;

namespace core {
	class ScopedTimer
	{
	public:
		explicit ScopedTimer(const char* function, std::string_view message = {})
			: m_function(function)
			, m_message(message)
			, m_start(std::chrono::steady_clock::now())
		{
		}

		~ScopedTimer()
		{
			const auto elapsed = std::chrono::steady_clock::now() - m_start;
			profiler::print(m_function, m_message, std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed));
		}

		ScopedTimer(const ScopedTimer&) = delete;
		ScopedTimer& operator=(const ScopedTimer&) = delete;
		ScopedTimer(ScopedTimer&&) = delete;
		ScopedTimer& operator=(ScopedTimer&&) = delete;

	private:
		const char* m_function;
		std::string_view m_message;
		std::chrono::steady_clock::time_point m_start;
	};
}

#define PROFILER_TIMER_CONCAT_INNER(a, b) a##b
#define PROFILER_TIMER_CONCAT(a, b)       PROFILER_TIMER_CONCAT_INNER(a, b)

#define PROFILER_TIMER(...)                                                     \
	const ::core::ScopedTimer PROFILER_TIMER_CONCAT(_profiler_timer_, __LINE__) \
	{                                                                           \
		__func__ __VA_OPT__(, ) __VA_ARGS__                                     \
	}
