#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


#include "RISCV.h"
#include "UI.h"


class Application{
public:
	Application();
	~Application();
	void Run();

private:
	RISCV m_riscv;

	GLFWwindow* m_window = NULL;
};
