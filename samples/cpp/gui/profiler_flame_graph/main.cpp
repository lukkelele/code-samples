#include <cstdint>
#include <cstdio>

#include "core/profiler.h"
#include "gui/window.h"

namespace {
	volatile std::uint64_t sink = 0;

	void busy(const std::uint64_t iterations)
	{
		std::uint64_t acc = 0;
		for (std::uint64_t idx = 0; idx < iterations; idx++) {
			acc += idx * 2654435761U;
		}
		sink = acc;
	}

	void integrate()
	{
		PROFILER_SCOPED();
		busy(200000);
	}

	void detect_collisions()
	{
		PROFILER_SCOPED();
		busy(1500000);
	}

	void update_physics()
	{
		PROFILER_SCOPED();
		integrate();
		detect_collisions();
	}

	void pathfinding()
	{
		PROFILER_SCOPED();
		busy(400000);
	}

	void update_ai()
	{
		PROFILER_SCOPED();
		pathfinding();
	}

	void cull()
	{
		PROFILER_SCOPED();
		busy(150000);
	}

	void draw()
	{
		PROFILER_SCOPED();
		busy(600000);
	}

	void render()
	{
		PROFILER_SCOPED();
		cull();
		draw();
	}

	void update_frame()
	{
		PROFILER_SCOPED();
		update_physics();
		update_ai();
		render();
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

	LOG_INFO("Enter main loop");
	while (window.is_running()) {
		{
			PROFILER_SCOPED("Main Loop");
			window.begin_frame();
			update_frame();
			window.end_frame();
		}
		PROFILER_MARK_FRAME();
	}

	window.destroy();
	return 0;
}
