#pragma once

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstddef>
#include <concepts>
#include <mutex>
#include <functional>
#include <print>
#include <thread>
#include <optional>
#include <type_traits>

namespace core {

	/**
	 * @fixme: This is kiiiinda bad.
	 * I don't like the detachment and the use of std::function.
	 */
	template<typename thread_impl = std::jthread>
	class thread
	{
	public:
		using handle_t = std::uint64_t;

		template<typename Func, typename... Args>
		thread(Func&& func, Args&&... args)
		{
			handle = total++;
			std::println("[thread::{}] created", handle);
			task = [func = std::forward<Func>(func), ... args = std::forward<Args>(args)]() mutable
			{
				std::invoke(func, std::move(args)...);
			};
		}
		thread() = delete;
		~thread() = default;
		thread(const thread&) = delete;
		thread(thread&&) = delete;

		thread& operator=(const thread&) = delete;
		thread& operator=(thread&&) = delete;

		void run()
		{
			assert(worker.joinable() == false && "thread already running");
			assert(task.has_value() && "thread has no task");

			worker = thread_impl([this]
			{
				running.store(true);
				std::println("[thread::{}] invoke task", handle);
				(*task)();
				running.store(false);
			});

			std::println("[thread::{}] detach worker", handle);
			worker.detach();
		}

		void join()
		{
			if (worker.joinable()) {
				worker.join();
			}
		}

		bool is_running() const
		{
			return running.load();
		}

		template<typename TRep, typename TPeriod>
		static void sleep(const std::chrono::duration<TRep, TPeriod> duration)
		{
			std::this_thread::sleep_for(duration);
		}

	private:
		handle_t handle = 0;
		thread_impl worker;
		std::optional<std::function<void()>> task;
		std::atomic_bool running;

		static inline std::uint32_t total = 0;
	};
}
