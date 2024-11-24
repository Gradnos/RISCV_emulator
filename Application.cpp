#include "Application.h"
#include "Console.h"

static void processInput(GLFWwindow* window);

Application::Application(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(1, 1, "LearnOpenGL", NULL, NULL);


    if (m_window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ;
    }

    Console::initConsole();
    UI::initUI(m_window, &m_riscv);
    Console::log("Init Application");
   
}



void Application::Run() {
    while (!glfwWindowShouldClose(m_window)){
        processInput(m_window);
        UI::drawUI();
        glfwPollEvents();
    }
}

Application::~Application(){  
    UI::deleteUI();
    glfwTerminate();
}


static void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_Escape))
        glfwSetWindowShouldClose(window, true);
}