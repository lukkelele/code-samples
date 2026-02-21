#include <cstdio>
#include <chrono>
#include <thread>

static void thread_func_a()
{
	for (int i = 0; i < 15; i++) {
		std::printf("[thread_func_a] i=%d\n", i);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

static void thread_func_b()
{
	for (int i = 0; i < 15; i++) {
		std::printf("[thread_func_b] i=%d\n", i);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main()
{
	std::printf("\n==============================\n");
	std::printf("\tsample: threads\n");
	std::printf("==============================\n\n");

	std::printf("creating threads\n");
	std::thread thread_a(thread_func_a);
	std::thread thread_b(thread_func_b);

	/* Wait until both threads are done running before exiting. */
	thread_a.join();
	thread_b.join();

	return 0;
}
