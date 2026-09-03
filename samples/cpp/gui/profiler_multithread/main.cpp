#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "core/profiler.h"
#include "gui/window.h"

namespace {
	constexpr int MAX_WORKERS = 256;
	constexpr int OVERSUBSCRIPTION_FACTOR = 8;
	constexpr int FALSE_SHARING_ITERATIONS = 5'000'000;

	PROFILER_LOCKABLE(std::mutex, shared_mtx);

	std::atomic<std::uint64_t> packed_counters[MAX_WORKERS];

	struct WorkerControls
	{
		std::atomic<bool> running{false};
		std::atomic<int> thread_count{1};
		std::atomic<bool> lock_contention{false};
		std::atomic<bool> uneven_load{false};
		std::atomic<bool> false_sharing{false};
		std::atomic<bool> oversubscription{false};
		std::atomic<int> work_size{40000};
		std::atomic<std::uint64_t> shared_counter{0};
	};

	std::uint64_t count_primes(const std::uint64_t limit)
	{
		std::uint64_t count = 0;
		for (std::uint64_t i = 2; i < limit; i++) {
			bool is_prime = true;
			for (std::uint64_t j = 2; j * j <= i; j++) {
				if (i % j == 0) {
					is_prime = false;
					break;
				}
			}
			if (is_prime) {
				count++;
			}
		}
		return count;
	}

	void worker_loop(const int id, const std::string& name, WorkerControls& ctl)
	{
		PROFILER_THREAD(name.c_str());
		while (ctl.running.load(std::memory_order_relaxed)) {
			PROFILER_SCOPED("Worker Iteration");
			std::uint64_t limit = static_cast<std::uint64_t>(ctl.work_size.load(std::memory_order_relaxed));
			if (ctl.uneven_load.load(std::memory_order_relaxed)) {
				limit *= static_cast<std::uint64_t>(id + 1);
			}

			std::uint64_t primes = 0;
			{
				PROFILER_SCOPED("Counting Primes");
				primes = count_primes(limit);
			}

			if (ctl.lock_contention.load(std::memory_order_relaxed)) {
				std::lock_guard<PROFILER_LOCKABLE_BASE(std::mutex)> lock(shared_mtx);
				PROFILER_SCOPED("Critical Section");
				ctl.shared_counter.fetch_add(count_primes(limit / 4), std::memory_order_relaxed);
			} else {
				ctl.shared_counter.fetch_add(primes, std::memory_order_relaxed);
			}

			if (ctl.false_sharing.load(std::memory_order_relaxed)) {
				PROFILER_SCOPED("Counter Update");
				std::atomic<std::uint64_t>& counter = packed_counters[id];
				for (int idx = 0; idx < FALSE_SHARING_ITERATIONS; idx++) {
					counter.fetch_add(1, std::memory_order_relaxed);
				}
			}

			PROFILER_PLOT("Primes / Iteration", static_cast<double>(primes));
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	class WorkerPool
	{
	public:
		~WorkerPool() { stop(); }

		[[nodiscard]] bool is_running() const { return ctl.running.load(); }
		[[nodiscard]] WorkerControls& controls() { return ctl; }

		void start(const int count)
		{
			stop();
			ctl.running.store(true);
			ctl.thread_count.store(count);

			names.clear();
			names.reserve(static_cast<std::size_t>(count));
			for (std::size_t idx = 0; idx < static_cast<std::size_t>(count); idx++) {
				names.emplace_back("Worker " + std::to_string(idx));
			}

			threads.reserve(static_cast<std::size_t>(count));
			for (std::size_t idx = 0; idx < static_cast<std::size_t>(count); idx++) {
				threads.emplace_back(worker_loop, static_cast<int>(idx), std::cref(names[idx]), std::ref(ctl));
			}

			LOG_INFO("Started {} worker(s)", count);
			PROFILER_MESSAGE_L("Worker pool started");
		}

		void stop()
		{
			ctl.running.store(false);
			for (auto& thread : threads) {
				if (thread.joinable()) {
					thread.join();
				}
			}
			if (!threads.empty()) {
				threads.clear();
				LOG_INFO("Stopped worker pool");
				PROFILER_MESSAGE_L("Worker pool stopped");
			}
		}

	private:
		WorkerControls ctl{};
		std::vector<std::thread> threads;
		std::vector<std::string> names;
	};
}

int main(const int argc, char* const* argv)
{
	core::init(lklog::level::trace);

	using namespace gui;
	const WindowSpecification window_spec = {
		.width = 1024,
		.height = 800,
		.title = SAMPLE_NAME};
	auto& window = CWindow::get();
	window.init(window_spec);

	PROFILER_THREAD("Main");

	const int hardware_threads = static_cast<int>(std::thread::hardware_concurrency());
	int thread_count = hardware_threads > 0 ? hardware_threads : 4;

	WorkerPool pool;

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		{
			PROFILER_SCOPED("Main Loop");
			window.begin_frame();

			if (ImGui::Begin("Tracy Multithreading")) {
				PROFILER_SCOPED("Control Panel");

				WorkerControls& ctl = pool.controls();
				const bool oversubscribed = ctl.oversubscription.load();
				const int active_count = oversubscribed
					? std::min(hardware_threads * OVERSUBSCRIPTION_FACTOR, MAX_WORKERS)
					: thread_count;

				bool running = pool.is_running();
				if (ImGui::Checkbox("Run workers", &running)) {
					if (running) {
						pool.start(active_count);
					} else {
						pool.stop();
					}
				}

				if (ImGui::SliderInt("Threads", &thread_count, 1, 32)) {
					if (pool.is_running() && !oversubscribed) {
						pool.start(thread_count);
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Number of worker threads. Each appears as its own row in the Tracy timeline.");
				}

				bool lock_contention = ctl.lock_contention.load();
				if (ImGui::Checkbox("Lock contention", &lock_contention)) {
					ctl.lock_contention.store(lock_contention);
					PROFILER_MESSAGE_L(lock_contention ? "Lock contention enabled" : "Lock contention disabled");
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Workers commit results inside one shared mutex.\nTracy lock view: long orange 'blocked' spans while threads wait.");
				}

				bool uneven_load = ctl.uneven_load.load();
				if (ImGui::Checkbox("Uneven load balancing", &uneven_load)) {
					ctl.uneven_load.store(uneven_load);
					PROFILER_MESSAGE_L(uneven_load ? "Uneven load enabled" : "Uneven load disabled");
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Worker N does (N+1)x the work.\nTimeline: a staircase of zone widths, light threads idle while stragglers run.");
				}

				bool false_sharing = ctl.false_sharing.load();
				if (ImGui::Checkbox("False sharing", &false_sharing)) {
					ctl.false_sharing.store(false_sharing);
					PROFILER_MESSAGE_L(false_sharing ? "False sharing enabled" : "False sharing disabled");
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Each worker hammers its own counter, but the counters share a cache line.\nTracy: wide 'Counter Update' zones despite no logical sharing.");
				}

				bool oversubscription = ctl.oversubscription.load();
				if (ImGui::Checkbox("Oversubscription", &oversubscription)) {
					ctl.oversubscription.store(oversubscription);
					PROFILER_MESSAGE_L(oversubscription ? "Oversubscription enabled" : "Oversubscription disabled");
					if (pool.is_running()) {
						pool.start(oversubscription
								? std::min(hardware_threads * OVERSUBSCRIPTION_FACTOR, MAX_WORKERS)
								: thread_count);
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Spawns %dx more threads than cores.\nTimeline: choppy, time-sliced zones as the scheduler thrashes.", OVERSUBSCRIPTION_FACTOR);
				}

				int work_size = ctl.work_size.load();
				if (ImGui::SliderInt("Work size", &work_size, 1000, 200000)) {
					ctl.work_size.store(work_size);
				}

				ImGui::Separator();
				ImGui::Text("Active workers: %d", pool.is_running() ? ctl.thread_count.load() : 0);
				ImGui::Text("Shared counter: %llu", static_cast<unsigned long long>(ctl.shared_counter.load()));
			}
			ImGui::End();

			PROFILER_PLOT("Active workers", static_cast<double>(pool.is_running() ? thread_count : 0));

			window.end_frame();
		}
		PROFILER_MARK_FRAME();
	}

	pool.stop();
	window.destroy();
	return 0;
}
