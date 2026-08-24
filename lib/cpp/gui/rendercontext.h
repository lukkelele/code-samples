#pragma once

#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/core.h"

#define LK_OpenGL(OpenGLFunction)                                                            \
	::gui::opengl::internal::check_for_errors();                                             \
	OpenGLFunction;                                                                          \
	ASSERT(::gui::opengl::internal::verify_func_result(#OpenGLFunction, __FILE__, __LINE__))

namespace gui::opengl::internal {
	inline void check_for_errors()
	{
		while (glGetError() != GL_NO_ERROR) {}
	}

	inline bool verify_func_result(const char* const func, const char* const file, const int line)
	{
		while (const GLenum err = glGetError()) {
			std::printf("Error: %d\n * Function: %s\n * File: %s\n * Line: %d\n", static_cast<int>(err), func, file, line);
			return false;
		}
		return true;
	}
}

namespace gui {
	inline glm::vec4 clear_color = {0.0f, 0.0f, 0.0f, 1.0f};

	void init_render_context(GLFWwindow* ctx);
	void new_frame();
	void end_frame();
}
