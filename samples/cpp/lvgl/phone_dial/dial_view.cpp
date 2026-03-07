#include "dial_view.h"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <print>
#include <src/core/lv_obj_style.h>
#include <string>

#include <lvgl/lvgl.h>

#include "global.h"

static lv_obj_t* root;
static lv_obj_t* textarea;
static lv_obj_t* button_matrix;

static constexpr std::array<const char*, 16> KEYPAD_MAP = {
	"1", "2", "3", "\n",
	"4", "5", "6", "\n",
	"7", "8", "9", "\n",
	LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_CALL, ""
};

static void on_button_matrix_event(lv_event_t* e)
{
	if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) {
		return;
	}

	lv_obj_t* button_matrix = lv_event_get_target_obj(e);
	const uint32_t button = lv_btnmatrix_get_selected_btn(button_matrix);
	const char* text = lv_btnmatrix_get_btn_text(button_matrix, button);
	if (text) {
		std::println("button: {}", text);
		if (std::strcmp(text, "1") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "2") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "3") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "4") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "5") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "6") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "7") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "8") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "9") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, "0") == 0) {
			lv_textarea_add_text(textarea, text);
		} else if (std::strcmp(text, LV_SYMBOL_BACKSPACE) == 0) {
			lv_textarea_delete_char(textarea);
		} else if (std::strcmp(text, LV_SYMBOL_CALL) == 0) {
			lv_textarea_set_text(textarea, "");

			ui::next_view = ui::VIEW_CONTACTS;
		}
	}
}

void dial_view::init()
{
	root = lv_obj_create(nullptr);
	lv_obj_set_layout(root, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_style_pad_hor(root, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_ver(root, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

	textarea = lv_textarea_create(root);
	lv_obj_set_width(textarea, lv_pct(100));
	lv_obj_set_flex_grow(textarea, 0);
	lv_obj_set_style_text_font(textarea, &lv_font_montserrat_48, 0);
	lv_textarea_set_placeholder_text(textarea, "");
	lv_textarea_set_one_line(textarea, true);
	lv_textarea_set_cursor_click_pos(textarea, false);

	button_matrix = lv_btnmatrix_create(root);
	lv_btnmatrix_set_map(button_matrix, KEYPAD_MAP.data());
	lv_obj_set_width(button_matrix, lv_pct(100));
	lv_obj_set_flex_grow(button_matrix, 1);
	lv_obj_add_event_cb(button_matrix, on_button_matrix_event, LV_EVENT_VALUE_CHANGED, nullptr);

	lv_obj_set_style_text_font(button_matrix, &lv_font_montserrat_32, LV_PART_ITEMS);
	lv_obj_set_style_bg_color(button_matrix, lv_palette_darken(LV_PALETTE_LIGHT_GREEN, 2), LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(button_matrix, lv_palette_darken(LV_PALETTE_GREEN, 2), LV_PART_ITEMS | LV_STATE_PRESSED);
}

void dial_view::enter()
{
	std::println("{}", __PRETTY_FUNCTION__);
	assert(root && "missing root");

	static constexpr bool DELETE_OLD_SCREEN = false;
	static constexpr lv_screen_load_anim_t ANIMATION = LV_SCREEN_LOAD_ANIM_OUT_BOTTOM;
	lv_screen_load_anim(root, ANIMATION, 220, 0, DELETE_OLD_SCREEN);
}

void dial_view::tick()
{
}
