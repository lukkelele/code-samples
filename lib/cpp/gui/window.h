#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "core/core.h"

namespace gui {
	struct WindowSpecification
	{
		std::uint16_t width = 1024;
		std::uint16_t height = 800;
		std::string title = "code-samples";
		bool vsync = true;
		bool start_maximized = false;
	};

	struct WindowData
	{
		std::uint16_t width = 0;
		std::uint16_t height = 0;
		std::string title;
		bool vsync = false;
		class CWindow* ref = nullptr;
	};

	class CWindow
	{
	private:
		CWindow() = default;

	public:
		~CWindow() = default;
		CWindow(CWindow&&) = delete;
		CWindow(const CWindow&) = delete;

		CWindow& operator=(CWindow&&) = delete;
		CWindow& operator=(const CWindow&) = delete;

		static CWindow& get();

		void init(const WindowSpecification& Spec);
		void destroy();

		void begin_frame();
		void end_frame();
		[[nodiscard]] bool is_running() const;

		[[nodiscard]] std::uint16_t get_width() const { return data.width; }
		[[nodiscard]] std::uint16_t get_height() const { return data.height; }
		void set_size(std::uint16_t width, std::uint16_t height);
		void set_title(std::string_view title);
		void set_vsync(bool enabled);
		[[nodiscard]] std::uint16_t get_refresh_rate() const;
		void maximize();
		void restore();
		bool is_maximized() const;

		[[nodiscard]] const WindowData& get_data() const { return data; }
		[[nodiscard]] GLFWwindow* get_glfw_window() const { return glfw_window; }

	private:
		void centralize();

	private:
		GLFWwindow* glfw_window = nullptr;
		WindowData data{};
	};
}
