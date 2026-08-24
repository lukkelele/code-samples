#include "window.h"

#include "rendercontext.h"

namespace gui {
	constexpr int OPENGL_MAJOR = 4;
	constexpr int OPENGL_MINOR = 6;
	constexpr int MSAA_SAMPLES = 4;

	CWindow& CWindow::get()
	{
		static CWindow window;
		return window;
	}

	void CWindow::init(const WindowSpecification& spec)
	{
		data.width = spec.width;
		data.height = spec.height;
		data.title = spec.title;

		const int glfw_init = glfwInit();
		ASSERT(glfw_init != 0);
		glfwSetErrorCallback([](const int err, const char* desc)
		{
			if (err == GLFW_FEATURE_UNAVAILABLE) {
				LOG_WARN_TAG("Window", "GLFW error ({}): {}", err, desc);
			} else {
				LOG_ERROR_TAG("Window", "GLFW error ({}): {}", err, desc);
			}
		});

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);
#ifdef LOG_BUILD_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		LOG_DEBUG_TAG("Window", "Create: {}x{}", data.width, data.height);
		data.ref = this;
		glfw_window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		ASSERT(glfw_window);
		glfwMakeContextCurrent(glfw_window);
		glfwFocusWindow(glfw_window);
		glfwSetWindowUserPointer(glfw_window, &data);

		set_vsync(spec.vsync);

		glfwSetWindowSizeCallback(glfw_window, [](GLFWwindow* window, const int width, const int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			ASSERT(data.ref, "Invalid window reference");
			data.ref->set_size(width, height);
		});

		glfwSetWindowCloseCallback(glfw_window, [](GLFWwindow* window)
		{
			LOG_TRACE_TAG("Window", "Set close flag");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		});

		centralize();
		if (spec.start_maximized) {
			glfwMaximizeWindow(glfw_window);
		}

		init_render_context(glfw_window);

		glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow*, const int width, const int height)
		{
			glViewport(0, 0, width, height);
		});
	}

	void CWindow::destroy()
	{
		LOG_TRACE_TAG("Window", "Destroy");
		glfwTerminate();
		glfw_window = nullptr;
	}

	void CWindow::begin_frame()
	{
		glfwPollEvents();
		gui::new_frame();
	}

	void CWindow::end_frame()
	{
		gui::end_frame();
		glfwSwapBuffers(glfw_window);
	}

	bool CWindow::is_running() const
	{
		return !glfwWindowShouldClose(glfw_window);
	}

	void CWindow::set_size(const std::uint16_t width, const std::uint16_t height)
	{
		if ((data.width == width) && (data.height == height)) {
			return;
		}

		data.width = width;
		data.height = height;

		if (glfw_window) {
			int current_w = 0;
			int current_h = 0;
			glfwGetWindowSize(glfw_window, &current_w, &current_h);
			if ((current_w != static_cast<int>(width)) || (current_h != static_cast<int>(height))) {
				if (is_maximized()) {
					glfwRestoreWindow(glfw_window);
				}
				glfwSetWindowSize(glfw_window, width, height);
				centralize();
			}
		}

		/* @todo: Resized callback */
	}

	void CWindow::set_title(std::string_view title)
	{
		ASSERT(glfw_window);
		LOG_DEBUG_TAG("Window", "Set title: {}", title);
		glfwSetWindowTitle(glfw_window, title.data());
	}

	void CWindow::set_vsync(const bool enabled)
	{
		LOG_DEBUG_TAG("Window", "VSync {}", enabled ? "enabled" : "disabled");
		glfwSwapInterval(enabled);
		data.vsync = enabled;
	}

	std::uint16_t CWindow::get_refresh_rate() const
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		ASSERT(mode);
		return mode->refreshRate;
	}

	void CWindow::maximize()
	{
		if (glfw_window && !is_maximized()) {
			glfwMaximizeWindow(glfw_window);
		}
	}

	void CWindow::restore()
	{
		if (glfw_window && is_maximized()) {
			glfwRestoreWindow(glfw_window);
		}
	}

	bool CWindow::is_maximized() const
	{
		return glfwGetWindowAttrib(glfw_window, GLFW_MAXIMIZED) == GLFW_TRUE;
	}

	void CWindow::centralize()
	{
		GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
		ASSERT(video_mode);

		int width = 0;
		int height = 0;
		glfwGetWindowSize(glfw_window, &width, &height);

		int monitor_x = 0;
		int monitor_y = 0;
		glfwGetMonitorPos(primary_monitor, &monitor_x, &monitor_y);

		const int pos_x = monitor_x + (video_mode->width - width) / 2;
		const int pos_y = monitor_y + (video_mode->height - height) / 2;

		glfwSetWindowPos(glfw_window, pos_x, pos_y);
		LOG_TRACE_TAG("Window", "Centered at ({}, {})", pos_x, pos_y);
	}
}
