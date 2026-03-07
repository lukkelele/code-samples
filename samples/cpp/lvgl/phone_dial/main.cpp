#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>
#include <print>
#include <string>
#include <vector>

#include "core/lvgl_core.h"
#include "core/demo_shop.h"

#include "dial_view.h"

static constexpr std::array<const char*, 16> KEYPAD_MAP = {
	"1", "2", "3", "\n",
	"4", "5", "6", "\n",
	"7", "8", "9", "\n",
	LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_CALL, ""
};

static void set_theme()
{
	lv_display_t* display = lv_display_get_default();
	lv_theme_t* theme = lv_theme_default_init(
		display,
		lv_palette_main(LV_PALETTE_LIGHT_BLUE),
		lv_palette_main(LV_PALETTE_GREY),
		true,
		LV_FONT_DEFAULT);

	lv_display_set_theme(display, theme);
}

int main()
{
	std::printf("\n==============================\n");
	std::printf("  sample: lvgl: phone dial\n");
	std::printf("==============================\n\n");

	lvgl::init();
	set_theme();

#if 0
	lv_obj_t* root = lv_screen_active();

	lv_obj_t* tv = lv_tabview_create(root);
	lv_tabview_set_tab_bar_position(tv, LV_DIR_BOTTOM);
	constexpr std::size_t tab_bar_size = 60;
	lv_tabview_set_tab_bar_size(tv, tab_bar_size);
	lv_obj_set_size(tv, lv_pct(100), lv_pct(100));

	lv_obj_t* tab1 = lv_tabview_add_tab(tv, "Keypad");
	lv_obj_t* tab2 = lv_tabview_add_tab(tv, "Shop");
	lv_obj_set_size(tab2, lv_pct(100), lv_pct(100));
	lv_obj_set_layout(tab2, LV_LAYOUT_FLEX);
	lv_obj_set_style_pad_all(tab2, 0, 0);

	demo::shop(tab2);

	lv_obj_t* keypad_view = tab1;
	// lv_obj_t* keypad_view = lv_obj_create(root);
	// lv_obj_t* keypad_view = lv_obj_create(NULL);
	lv_obj_set_layout(keypad_view, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(keypad_view, LV_FLEX_FLOW_COLUMN);

	create_keypad(keypad_view);
	// lv_screen_load(keypad_view);
#endif

	std::vector<std::unique_ptr<lvgl::view>> views;
	auto& dial = views.emplace_back(std::make_unique<dial_view>());

	for (auto& view : views) {
		view->init();
	}

	dial->enter();

	while (true) {
		for (auto& view : views) {
			view->tick();
		}
		lvgl::tick();
	}

	return 0;
}
