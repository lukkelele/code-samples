#include <cstdio>

#include "gui/window.h"

int main(const int argc, char* const* argv)
{
	core::init(lklog::level::trace);

	using namespace gui;
	const WindowSpecification window_spec = {
		.width = 800,
		.height = 600,
		.title = "simple_window"};
	auto& window = CWindow::get();
	window.init(window_spec);

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		window.begin_frame();

		if (ImGui::Begin("Window")) {
			if (ImGui::Button("Button")) {
				LOG_INFO("Pressed button");
			}
			ImGui::End();
		}

		window.end_frame();
	}

	window.destroy();
	return 0;
}
