#include <algorithm>
#include <chrono>
#include <cstdint>
#include <print>
#include <span>
#include <thread>
#include <vector>

#include "core/core.h"
#include "core/timer.h"

static constexpr std::uint64_t GOLDEN = 0x9E3779B97F4A7C15;

static std::uint64_t compute_checksum(const std::span<const std::uint64_t> data)
{
	PROFILER_TIMER();
	std::uint64_t acc = 0;
	for (const std::uint64_t v : data) {
		acc ^= v + GOLDEN + (acc << 6) + (acc >> 2);
	}
	return acc;
}

static std::uint64_t build_and_sort(const std::size_t n)
{
	PROFILER_TIMER("sort + reduce");
	std::vector<std::uint64_t> values(n);
	for (std::size_t i = 0; i < n; i++) {
		values[i] = (n - i) * GOLDEN;
	}
	std::sort(values.begin(), values.end());

	std::uint64_t acc = 0;
	for (const std::uint64_t v : values) {
		acc += v;
	}
	return acc;
}

static void simulate_io()
{
	PROFILER_TIMER("Blocking IO");
	std::this_thread::sleep_for(3ms);
}

int main()
{
	core::print_banner("profiler");

	std::vector<std::uint64_t> data(2'000);
	for (std::size_t i = 0; i < data.size(); i++) {
		data[i] = i * GOLDEN;
	}

	std::uint64_t sink = 0;

	{
		PROFILER_TIMER("Warmup block");
		for (int pass = 0; pass < 4; pass++) {
			sink ^= compute_checksum(data);
		}
	}

	{
		PROFILER_TIMER("lukas long test code");
		std::this_thread::sleep_for(1200ms);
	}

	{
		PROFILER_TIMER("lukas small test code");
		std::this_thread::sleep_for(600ms);
	}

	{
		PROFILER_TIMER();
		std::this_thread::sleep_for(600ms);
	}

	sink ^= build_and_sort(200'000);
	simulate_io();

	sink ^= compute_checksum(data);
	simulate_io();

	sink ^= compute_checksum(data);
	simulate_io();

	std::println("\nsink = {}", sink);
	return 0;
}
