#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int WIDTH = 1200;
bool consoleOpen = true;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);




    GLFWwindow* window = glfwCreateWindow(1, 1, "LearnOpenGL", NULL, NULL);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());


    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");


    static char text[1024 * 16];

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(WIDTH, 800));

        ImGuiWindowFlags window_flags = 0;
        //window_flags |= ImGuiWindowFlags_NoMove;
        //window_flags |= ImGuiWindowFlags_NoResize;
       // window_flags |= ImGuiWindowFlags_NoCollapse;
        //window_flags |= ImGuiWindowFlags_NoDecoration;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        
        ImGui::SetNextWindowCollapsed(true);
        ImGui::Begin("hello",(bool*)0 , window_flags);
        bool hovered = ImGui::IsMouseHoveringRect(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowSize().x + ImGui::GetWindowPos().x,ImGui::GetWindowPos().y + 20), false);

        ImGui::BeginMenuBar();
        ImGui::Text("Text Editor // ");
        if (ImGui::Button("console")) {
            consoleOpen = !consoleOpen;
        }
        if (hovered) ImGui::Text("hov");
        else ImGui::Text("not hov");
        float buttonWidth1 = ImGui::CalcTextSize(" X ").x;
        float buttonWidth2 = ImGui::CalcTextSize(" [] ").x;

        float widthNeeded = buttonWidth1 + buttonWidth2 + style.ItemSpacing.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - style.ItemSpacing.x);
        if (ImGui::Button(" [] ")) {
            int xpos, ypos, width, height;
            bool maximized = false;
            glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
            if (ImGui::GetWindowSize().x == width && ImGui::GetWindowSize().y == height &&
                ImGui::GetWindowPos().x == xpos && ImGui::GetWindowPos().y == ypos)
                maximized = true;
            if (maximized) {
                width = 800;
                height = 600;
            }
            ImGui::SetWindowSize(ImVec2(width, height));
            ImGui::SetWindowPos(ImVec2(0, 0));
        }


        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - style.ItemSpacing.x);
        if (ImGui::Button(" X ")) {
            glfwSetWindowShouldClose(window, true);
        }

        ImGui::MenuItem("hellow", "C", false, true);
        ImGui::EndMenuBar();

        




        if (consoleOpen) {
            ImGui::Begin("Console", (bool*)0, window_flags);
            ImGui::BeginMenuBar();
            ImGui::Text("console");
            float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

            float widthNeeded = buttonWidth1 + buttonWidth1;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
            if (ImGui::Button(" X ")) {
                consoleOpen = false;
            }
            ImGui::EndMenuBar();
            ImGui::End();
        }

        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35));
        ///ImGui::InputTextMultiline(NULL, text.get(), 1000);
        //glfwSetWindowSize(window, ImGui::GetWindowSize().x + 4, ImGui::GetWindowSize().y + 4);
        //glfwSetWindowPos(window, ImGui::GetWindowPos().x - 2, ImGui::GetWindowPos().y - 2);
        ImGui::End();


        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin("wow", (bool*)0, window_flags);
        ImGui::Text("hasada");
        ImGui::End();

        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_Escape))
        glfwSetWindowShouldClose(window, true);
}