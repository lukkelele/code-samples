#include "rendercontext.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

void gui::init_render_context(GLFWwindow* const ctx)
{
	LOG_INFO("Initializing render context");
	const int glad_init = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(glad_init != 0);
	LK_OpenGL(glEnable(GL_MULTISAMPLE));
	LK_OpenGL(glEnable(GL_BLEND));

	/* Initialize ImGui. */
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingAlwaysTabBar = false;
	ImGui_ImplGlfw_InitForOpenGL(ctx, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImGui::StyleColorsDark();
}

void gui::new_frame()
{
	LK_OpenGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	LK_OpenGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	LK_OpenGL(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a));

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void gui::end_frame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
