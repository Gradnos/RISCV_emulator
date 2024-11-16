#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void buildDock(const char* name, ImGuiID mainDockId);
void helpmee();
void split();
void consoleWindow(ImVec2 vec2);
void textEditor(ImVec2 vec2);
void visualiser(ImVec2 vec2);

int WIDTH = 1200;
bool consoleOpen = true;
bool visualiserOpen = true;
bool firstTime = true;

void ShowExampleAppDockSpace(bool* p_open);
static char text[1024 * 16];

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




    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar;


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));

        ImGui::Begin("Splitter test",(bool*)false, window_flags);

        ImVec2 mainSize = ImGui::GetWindowSize();

        ImGui::BeginMenuBar();
        ImGui::Text("RISCV emulator // ");
        if (ImGui::Button("console")) {
            consoleOpen = !consoleOpen;
        }
        if (ImGui::Button("visualiser")) {
            visualiserOpen = !visualiserOpen;
        }

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
        ImGui::EndMenuBar();


        static float ratioW = 0.8f;
        static float ratioH = 0.8f;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32_BLACK_TRANS);

        float w = mainSize.x * ratioW; //text editor width
        float h = mainSize.y * ratioH; // visualiser height

        if (!consoleOpen && !visualiserOpen) w = 0; // make text editor full width
        textEditor(ImVec2(w,0));
        if (consoleOpen || visualiserOpen) {
            ImGui::SameLine();
            ImGui::InvisibleButton("vsplitter", ImVec2(4.0f, -1));
            if (ImGui::IsItemHovered()) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }
            if (ImGui::IsItemActive()) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                w += ImGui::GetIO().MouseDelta.x;
                ratioW = w / mainSize.x;
                if (ratioW < 0.2) ratioW = 0.2;
                if (ratioW > 0.8) ratioW = 0.8;
            }

            ImGui::SameLine();
            ImGui::BeginChild("side", ImVec2(0, 0));

                if (visualiserOpen) {
                    ImGui::SameLine();
                    ImGui::SameLine();
                    if (!consoleOpen) h = 0; // if console not open and visualiser open make height full
                    visualiser(ImVec2(0, h));
                }
                if (consoleOpen == visualiserOpen) { //checks if both of them are open as one of them is true if we are on this line
                    ImGui::InvisibleButton("hsplitter", ImVec2(-1, 4.0f));
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                    }
                    if (ImGui::IsItemActive()) {
                        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                        h += ImGui::GetIO().MouseDelta.y;
                        ratioH = h / mainSize.y;
                        if (ratioH < 0.3) ratioH = 0.3;
                        if (ratioH > 0.8) ratioH = 0.8;
                    }
                }
                if (consoleOpen) {
                    consoleWindow(ImVec2(0, 0)); // fill whatever area is left
                }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::End();

        ImGui::PopStyleVar();

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
        if (firstTime)
            firstTime = false;
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


void consoleWindow(ImVec2 vec2) {
     ImGuiWindowFlags window_flags = 0;
     window_flags |= ImGuiWindowFlags_MenuBar;
     window_flags |= ImGuiWindowFlags_NoTitleBar;
     window_flags |= ImGuiWindowFlags_NoDecoration;
     //ImGui::SetNextWindowDockID(mainDockId);
     ImGui::BeginChild("console", vec2, true, window_flags);
         ImGui::BeginMenuBar();
         ImGui::Text("console");
         float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

         float widthNeeded = buttonWidth1 + buttonWidth1;
         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);

         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
         if (ImGui::Button(" X ")) {
            consoleOpen = false;
         }

         ImGui::EndMenuBar();
     ImGui::EndChild();
}

void textEditor(ImVec2 vec2) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    ImGui::BeginChild("textEditor", vec2, true, window_flags);
    ImGui::BeginMenuBar();
    ImGui::Text("textEditor");
    ImGui::EndMenuBar();

    ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35));
    ImGui::EndChild();
}


void visualiser(ImVec2 vec2) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    ImGui::BeginChild("visualiser", vec2, true, window_flags);
    ImGui::BeginMenuBar();
    ImGui::Text("visualiser");
    float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

    float widthNeeded = buttonWidth1 + buttonWidth1;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
    if (ImGui::Button(" X ")) {
        visualiserOpen = false;
    }
    ImGui::EndMenuBar();

    ImGui::EndChild();
}