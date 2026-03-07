#pragma once

#include "core/lvgl_core.h"

class dial_view : public lvgl::view
{
public:
	dial_view() = default;
	~dial_view() override = default;

	void init() override;
	void enter() override;
	void tick() override;
};
