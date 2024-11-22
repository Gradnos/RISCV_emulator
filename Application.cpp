#include "Application.h"


static void processInput(GLFWwindow* window);


Application::Application()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(1, 1, "LearnOpenGL", NULL, NULL);


    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ;
    }

    UI::initUI(m_window, &m_riscv);
}



void Application::Run() {
    while (!glfwWindowShouldClose(m_window))
    {


        processInput(m_window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        UI::drawUI();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

Application::~Application()
{   
    UI::deleteUI();
    glfwTerminate();
}


static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_Escape))
        glfwSetWindowShouldClose(window, true);
}


