#pragma once

#include "core/lvgl_core.h"

class contacts_view : public lvgl::view
{
public:
	contacts_view() = default;
	~contacts_view() override = default;

	void init() override;
	void enter() override;
	void tick() override;
};
