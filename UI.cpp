#include "UI.h"

namespace UI {

    void consoleWindow(ImVec2 vec2) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoDecoration;
        //ImGui::SetNextWindowDockID(mainDockId);
        ImGui::BeginChild("console", vec2, true, window_flags);
        ImGui::BeginMenuBar();
        ImGui::Text("console");
        float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

        int widthNeeded = buttonWidth1 + style.FramePadding.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        if (ImGui::Button(" X ")) {
            m_consoleOpen = false;
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

        ImGui::InputTextMultiline("##source", m_riscv->getTextPtr(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35));
        ImGui::EndChild();
    }


    static void visualiser(ImVec2 vec2) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoDecoration;
        ImGui::BeginChild("visualiser", vec2, true, window_flags);
        ImGui::BeginMenuBar();
        ImGui::Text("visualiser");
        float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

        int widthNeeded = buttonWidth1 + style.FramePadding.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        if (ImGui::Button(" X ")) {
            m_visualiserOpen = false;
        }
        ImGui::EndMenuBar();

        ImGui::EndChild();
    }

    void initUI(GLFWwindow* w, RISCV* r) {
        m_window = w;
        m_riscv = r;
        IMGUI_CHECKVERSION();
        m_ctx = ImGui::CreateContext();
        m_io = &ImGui::GetIO(); (void)m_io;

        m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        m_style = &ImGui::GetStyle();
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            m_style->WindowRounding = 0.0f;
            m_style->Colors[ImGuiCol_WindowBg].w = 1.0f;
        }


        ImGui_ImplGlfw_InitForOpenGL(w, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }


    void drawUI() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (m_maximized) {
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

        ImGui::Begin("Splitter test", (bool*)false, window_flags);
        ImVec2 mainSize = ImGui::GetWindowSize();


        ImGui::BeginMenuBar();
        ImGui::Text("RISCV emulator /// ");
        if (ImGui::Button("Console")) {
            m_consoleOpen = !m_consoleOpen;
        }
        ImGui::Text(" | ");
        if (ImGui::Button("Visualiser")) {
            m_visualiserOpen = !m_visualiserOpen;
        }
        ImGui::Text(" /// ");
        if (ImGui::Button(" Run > ")) {
            m_riscv->run();
        }
        if (ImGui::Button(" Step ~ ")) {
            m_riscv->step();
        }

        float buttonWidth1 = ImGui::CalcTextSize(" X ").x;
        float buttonWidth2 = ImGui::CalcTextSize(" [] ").x;
        ImGuiStyle& style = ImGui::GetStyle();
        float widthNeeded = buttonWidth1 + buttonWidth2 + style.FramePadding.x + style.ItemSpacing.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        int xpos, ypos, width, height;
        m_maximized = false;
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
        if (ImGui::GetWindowSize().x == width && ImGui::GetWindowSize().y == height &&
            ImGui::GetWindowPos().x == xpos && ImGui::GetWindowPos().y == ypos)
            m_maximized = true;
        if (ImGui::Button(" [] ")) {
            if (m_maximized) {
                width = 800;
                height = 600;
            }
            ImGui::SetWindowSize(ImVec2(width, height));
            ImGui::SetWindowPos(ImVec2(0, 0));
            m_maximized = !m_maximized;
        }
        widthNeeded = buttonWidth1 + style.FramePadding.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        if (ImGui::Button(" X ")) {
            glfwSetWindowShouldClose(m_window, true);
        }
        ImGui::EndMenuBar();


        static float ratioW = 0.8f;
        static float ratioH = 0.8f;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32_BLACK_TRANS);

        float w = mainSize.x * ratioW; //text editor width
        float h = mainSize.y * ratioH; // visualiser height

        if (!m_consoleOpen && !m_visualiserOpen) w = 0; // make text editor full width
        textEditor(ImVec2(w, 0));
        if (m_consoleOpen || m_visualiserOpen) {
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

            if (m_visualiserOpen) {
                ImGui::SameLine();
                ImGui::SameLine();
                if (!m_consoleOpen) h = 0; // if console not open and visualiser open make height full
                visualiser(ImVec2(0, h));
            }
            if (m_consoleOpen == m_visualiserOpen) { //checks if both of them are open as one of them is true if we are on this line
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
            if (m_consoleOpen) {
                consoleWindow(ImVec2(0, 0)); // fill whatever area is left
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::End();

        ImGui::PopStyleVar();
        //ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }

    void deleteUI() {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }
}
