#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RISCV.h"
#include "Console.h"




// using namespace instead of class because of imgui implementation
namespace UI {

	// public
	void initUI(GLFWwindow* w, RISCV* r);
	void drawUI();
	void deleteUI();



	//private
	static GLFWwindow* m_window = NULL;
	static ImGuiIO* m_io = NULL;
	static ImGuiStyle* m_style = NULL;
	static ImGuiContext* m_ctx = NULL;
	static RISCV* m_riscv = NULL;

	static int m_WIDTH = 1200;
	static int m_HEIGHT = 1000;
	static ImVec2 m_mainWindowSize = { 0,0 };

	static int m_workXpos, m_workYpos, m_workWidth, m_workHeight;

	static float m_sliderRatioV = 0.7f;
	static float m_sliderRatioH = 0.7f;

	static bool m_consoleOpen = true;
	static bool m_visualiserOpen = true;
	static bool m_maximized = true;


	static ImGuiWindowFlags m_myChildWindowFlags = 0 |ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration;
	static ImGuiWindowFlags m_myMainWindowFlags = 0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;

	static void consoleWindow(ImVec2 vec2);
	static void textEditor(ImVec2 vec2);
	static void visualiser(ImVec2 vec2);

	static void mainMenuBar();
	static void mainMenuEndButtons();
	static void mainWindow();

	static void verticalSplitter();
	static void horizontalSplitter();
	static void leftBar();

	// updates work area and also returns true if current window is maximized
	static bool updateWorkArea();
}