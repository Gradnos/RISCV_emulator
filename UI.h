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

#include "Util.h";



// using namespace instead of class because of imgui implementation
namespace UI {

	// public
	void initUI(GLFWwindow* w, RISCV* r);
	void drawUI();
	void deleteUI();
	void processInput();



	//private
	static GLFWwindow* m_window = NULL;
	static ImGuiIO* m_io = NULL;
	static ImGuiStyle* m_style = NULL;
	static ImGuiContext* m_ctx = NULL;
	static RISCV* m_riscv = NULL;

	static ImFont* m_mainFont = NULL;
	static const char* m_mainFontPath = ".\\ImGui\\JetBrainsMono-Medium.ttf";

	static int m_WIDTH = 1200;
	static int m_HEIGHT = 1000;
	static ImVec2 m_mainWindowSize = { 0,0 };

	static int m_workXpos, m_workYpos, m_workWidth, m_workHeight;

	static float m_sliderRatioV = 0.7f;
	static float m_sliderRatioH = 0.7f;
	static float m_regSliderRatio = 0.5f;

	static bool m_consoleOpen = true;
	static bool m_visualiserOpen = true;
	static bool m_maximized = true;

	static int m_currRegCol = -1;
	static int m_currRegRow = -1;

	static int m_currMemRow = -1;
	static int m_startMemRow = -1;
	static int m_selecMemEnd = -1;
	static int m_selecMemStart = -1;
	static bool m_selectingMem = false;
	static bool m_selectUp = false;
	static int m_selectedMemOffset = 0;
	static int m_selectLen = 1;

	static bool m_mouseWasDown = false;

	static ImGuiWindowFlags m_myChildWindowFlags = 0 |ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
	static ImGuiWindowFlags m_myMainWindowFlags = 0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
	static ImGuiInputTextFlags m_ConsoleInputFlags = 0 | ImGuiInputTextFlags_ReadOnly;


	static void consoleWindow(ImVec2 vec2);
	static void textEditor(ImVec2 vec2);
	static void visualiser(ImVec2 vec2);
	static void drawRegisters();
	static void registerSplitter();
	static void drawMemory();
	static void handleMemorySelect();

	static void mainMenuBar();
	static void mainMenuEndButtons();
	static void mainWindow();



	static void verticalSplitter();
	static void horizontalSplitter();
	static void leftBar();

	// updates work area and also returns true if current window is maximized
	static bool updateWorkArea();

	static ImU32 m_highlightCol;

}