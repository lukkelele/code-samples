#include <cstdio>

#include "core/profiler.h"
#include "gui/window.h"

static TracyLockable(std::mutex, mtx);
// static PROFILER_LOCKABLE(std::mutex, mtx);

static void thread_1()
{
	PROFILER_THREAD("Thread 1");
	LOG_INFO("Thread 1");
	while (true) {
		PROFILER_SCOPED();
		std::this_thread::sleep_for(10ms);
		std::lock_guard<LockableBase(std::mutex)> lock(mtx);
		std::this_thread::sleep_for(200ms);
	}
}

static void thread_2()
{
	PROFILER_THREAD("Thread 2");
	LOG_INFO("Thread 2");
	while (true) {
		PROFILER_SCOPED();
		std::this_thread::sleep_for(30ms);
		std::lock_guard<LockableBase(std::mutex)> lock(mtx);
		std::this_thread::sleep_for(10ms);
	}
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

	std::thread t1(thread_1);
	std::thread t2(thread_2);

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		{
			PROFILER_SCOPED("Main Loop");
			window.begin_frame();

			if (ImGui::Begin("Window")) {
				PROFILER_SCOPED("Window");
				static auto sleep_delay = 800ms;
				if (ImGui::Button("Sleep")) {
					LOG_INFO("Sleeping for {}", sleep_delay);
					std::this_thread::sleep_for(sleep_delay);
				}

				int delay = std::chrono::duration_cast<std::chrono::milliseconds>(sleep_delay).count();
				ImGui::TextUnformatted("Timeout");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(80);
				if (ImGui::SliderInt("##sleep_delay", &delay, 0, 3000)) {
					sleep_delay = std::chrono::milliseconds(delay);
				}
			}
			ImGui::End();

			window.end_frame();
		}
		PROFILER_MARK_FRAME();
	}

	t1.detach();
	t2.detach();

	window.destroy();
	return 0;
}
