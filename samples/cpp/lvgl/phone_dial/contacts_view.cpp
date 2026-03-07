#include "contacts_view.h"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <print>
#include <src/core/lv_obj_scroll.h>
#include <src/core/lv_obj_style.h>
#include <src/misc/lv_color.h>
#include <src/misc/lv_palette.h>
#include <string>

#include <lvgl/lvgl.h>

#include "global.h"

static lv_obj_t* root = nullptr;
static lv_obj_t* contact_list = nullptr;
static lv_obj_t* dock = nullptr;

/* placeholder */
static void on_event(lv_event_t* e)
{
}

static void create_contact_list(lv_obj_t* parent)
{
	assert(contact_list == nullptr);
	contact_list = lv_obj_create(parent);
	lv_obj_set_width(contact_list, LV_PCT(100));
	lv_obj_set_layout(contact_list, LV_LAYOUT_FLEX);
	lv_obj_set_flex_grow(contact_list, 1);
	lv_obj_set_flex_flow(contact_list, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_scroll_dir(contact_list, LV_DIR_VER);
}

static lv_obj_t* create_contact_entry(lv_obj_t* parent, const char* name)
{
	std::println("{}: {}", __PRETTY_FUNCTION__, name);
	lv_obj_t* row = lv_btn_create(parent);
	lv_obj_set_width(row, lv_pct(100));
	lv_obj_set_height(row, LV_SIZE_CONTENT);
	lv_obj_set_layout(row, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
	lv_obj_set_style_pad_all(row, 10, 0);

	lv_obj_add_flag(row, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_set_style_bg_color(row, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_STATE_CHECKED);
	lv_obj_set_style_text_color(row, lv_color_white(), LV_STATE_CHECKED);

	lv_obj_t* label = lv_label_create(row);
	lv_obj_center(label);
	lv_label_set_text(label, name);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_34, 0);

	return row;
}

static void on_button_dial(lv_event_t* e)
{
	std::println("{}", __PRETTY_FUNCTION__);
	ui::next_view = ui::VIEW_DIAL;
}

static void create_dock(lv_obj_t* parent)
{
	dock = lv_obj_create(root);
	lv_obj_set_width(dock, lv_pct(100));
	lv_obj_set_scrollbar_mode(dock, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_height(dock, 80);
	lv_obj_set_layout(dock, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(dock, LV_FLEX_FLOW_ROW);

	lv_obj_set_flex_align(
		dock,
		LV_FLEX_ALIGN_SPACE_EVENLY,
		LV_FLEX_ALIGN_CENTER,
		LV_FLEX_ALIGN_CENTER);

	lv_obj_t* button_dial = lv_btn_create(dock);
	lv_obj_t* label = lv_label_create(button_dial);
	lv_label_set_text(label, LV_SYMBOL_CALL);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_38, 0);
	lv_obj_add_event_cb(button_dial, on_button_dial, LV_EVENT_CLICKED, nullptr);
	lv_obj_set_style_radius(button_dial, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(button_dial, lv_palette_darken(LV_PALETTE_ORANGE, 2), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(button_dial, lv_palette_darken(LV_PALETTE_LIME, 2), LV_STATE_PRESSED);

	lv_obj_t* button_contacts = lv_btn_create(dock);
	label = lv_label_create(button_contacts);
	lv_label_set_text(label, LV_SYMBOL_KEYBOARD);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_38, 0);

	lv_obj_t* button_settings = lv_btn_create(dock);
	label = lv_label_create(button_settings);
	lv_label_set_text(label, LV_SYMBOL_SETTINGS);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_38, 0);
}

void contacts_view::init()
{
	root = lv_obj_create(nullptr);
	lv_obj_set_layout(root, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_style_pad_hor(root, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_ver(root, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

	create_contact_list(root);
	create_contact_entry(contact_list, "lukkelele");
	create_contact_entry(contact_list, "ludvig");
	create_contact_entry(contact_list, "ollebolle");
	create_contact_entry(contact_list, "linnea");
	create_contact_entry(contact_list, "yngve");
	create_contact_entry(contact_list, "katarina");
	create_contact_entry(contact_list, "kegan");

	create_dock(root);
}

void contacts_view::enter()
{
	std::println("{}", __PRETTY_FUNCTION__);
	assert(root && "missing root");
	// lv_screen_load(root);

	static constexpr bool DELETE_OLD_SCREEN = false;
	static constexpr lv_screen_load_anim_t ANIMATION = LV_SCREEN_LOAD_ANIM_OUT_TOP;
	lv_screen_load_anim(root, ANIMATION, 220, 0, DELETE_OLD_SCREEN);
}

void contacts_view::tick()
{
}
