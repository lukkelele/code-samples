#include "lvgl_core.h"

#include <cassert>
#include <print>

namespace lvgl {

	static lv_display_t* display = nullptr;
	static lv_indev_t* mouse = nullptr;
	static lv_indev_t* mousewheel = nullptr;
	static lv_indev_t* keyboard = nullptr;
	static lv_indev_t* cursor = nullptr;

	static void keyboard_cb(lv_event_t* event)
	{
		if (lv_event_get_code(event) != LV_EVENT_KEY) {
			return;
		}

		const uint32_t key = lv_event_get_key(event);
		std::println("[{}] key={}", __PRETTY_FUNCTION__, key);
		if (key == LV_KEY_ESC) {
			std::println("pressed escape");
		}
	}

	void init(const std::size_t width, const std::size_t height)
	{
		lv_init();

		display = lv_sdl_window_create(static_cast<int>(width), static_cast<int>(height));
		lv_display_set_default(display);
		lv_sdl_window_set_title(display, "lvgl: phone dial");

		mouse = lv_sdl_mouse_create();
		assert(mouse);
		lv_indev_set_group(mouse, lv_group_get_default());
		lv_indev_set_display(mouse, display);

		mousewheel = lv_sdl_mousewheel_create();
		assert(mousewheel);
		lv_indev_set_display(mousewheel, display);

		keyboard = lv_sdl_keyboard_create();
		assert(keyboard);
		lv_indev_set_display(keyboard, display);
		lv_indev_set_group(keyboard, lv_group_get_default());
		lv_indev_add_event_cb(keyboard, keyboard_cb, LV_EVENT_KEY, nullptr);
	}

	void tick()
	{
		uint32_t time_next = lv_timer_handler();
		if (time_next == LV_NO_TIMER_READY) {
			time_next = LV_DEF_REFR_PERIOD;
		}
		lv_delay_ms(time_next);
	}
}
