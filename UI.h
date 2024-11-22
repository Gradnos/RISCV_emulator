#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RISCV.h"




// using namespace instead of class because of imgui implementation
namespace UI {
	static GLFWwindow* m_window = NULL;
	static ImGuiIO* m_io = NULL;
	static ImGuiStyle* m_style = NULL;
	static ImGuiContext* m_ctx = NULL;
	static RISCV* m_riscv = NULL;

	static int m_WIDTH = 1200;
	static int m_HEIGHT = 1000;
	static bool m_consoleOpen = true;
	static bool m_visualiserOpen = true;
	static bool m_maximized = true;


	void consoleWindow(ImVec2 vec2);
	void textEditor(ImVec2 vec2);
	void visualiser(ImVec2 vec2);
	void initUI(GLFWwindow* w, RISCV* r);
	void drawUI();
	void deleteUI();
}

