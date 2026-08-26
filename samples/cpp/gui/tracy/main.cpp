#include <cstdio>

#include "gui/window.h"

#include <tracy/Tracy.hpp>

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

	tracy::SetThreadName("Main");

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		ZoneScopedN("Main Loop");
		window.begin_frame();

		if (ImGui::Begin("Window")) {
			ZoneScopedN("Window");
			if (ImGui::Button("Button")) {
				LOG_INFO("Pressed button");
				std::this_thread::sleep_for(500ms);
			}
			ImGui::End();
		}

		window.end_frame();
		FrameMark;
	}

	window.destroy();
	return 0;
}
