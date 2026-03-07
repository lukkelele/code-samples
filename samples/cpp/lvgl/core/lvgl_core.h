#pragma once

#include <cstdint>

#include <lvgl/lvgl.h>

namespace lvgl {
	void init(const std::size_t width = 600, const std::size_t height = 800);
	void tick();

	class view
	{
	protected:
		view() = default;

	public:
		virtual ~view() = default;
		view(const view&) = delete;
		view(view&&) = delete;

		view& operator=(const view&) = delete;
		view& operator=(view&&) = delete;

		virtual void init() = 0;
		virtual void enter() = 0;
		virtual void tick() = 0;
	};
}
