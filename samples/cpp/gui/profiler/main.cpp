#include <cstdio>

#include "core/profiler.h"
#include "gui/window.h"

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

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		{
			PROFILER_SCOPED("Main Loop");
			window.begin_frame();

			if (ImGui::Begin("Window")) {
				PROFILER_SCOPED("Window");
				if (ImGui::Button("Button")) {
					LOG_INFO("Pressed button");
					std::this_thread::sleep_for(300ms);
				}
			}
			ImGui::End();

			window.end_frame();
		}
		PROFILER_MARK_FRAME();
	}

	window.destroy();
	return 0;
}
