#include "UI.h"

namespace UI {

    void initUI(GLFWwindow* w, RISCV* r){
        m_window = w;
        m_riscv = r;
        IMGUI_CHECKVERSION();
        m_ctx = ImGui::CreateContext();
        m_io = &ImGui::GetIO(); (void)m_io;

        m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
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
        
        mainWindow();

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



    static void mainWindow() {
        if (m_maximized)
            m_myMainWindowFlags |= ImGuiWindowFlags_NoResize;
        else
            m_myMainWindowFlags &= ~ImGuiWindowFlags_NoResize;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

        ImGui::Begin("RISCV emulator", (bool*)false, m_myMainWindowFlags);
            m_mainWindowSize = ImGui::GetWindowSize();
            mainMenuBar();


            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32_BLACK_TRANS);

            float w = m_mainWindowSize.x * m_sliderRatioV; //text editor width
            if (!m_consoleOpen && !m_visualiserOpen) 
                w = 0; // make text editor full width
            textEditor(ImVec2(w, 0));

            if (m_consoleOpen || m_visualiserOpen) {
                verticalSplitter();
                leftBar();
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }



    static void mainMenuBar() {
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

            mainMenuEndButtons();

        ImGui::EndMenuBar();
    }

    static void mainMenuEndButtons() {
        float buttonWidth1 = ImGui::CalcTextSize(" X ").x;
        float buttonWidth2 = ImGui::CalcTextSize(" [] ").x;
        float widthNeeded = buttonWidth1 + buttonWidth2 + m_style->FramePadding.x + m_style->ItemSpacing.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        m_maximized = false;
        if (updateWorkArea())
            m_maximized = true;
        if (ImGui::Button(" [] ")) {
            int width = m_workWidth;
            int height = m_workHeight;
            if (m_maximized) {
                width = 800;
                height = 600;
            }
            ImGui::SetWindowSize(ImVec2(width, height));
            ImGui::SetWindowPos(ImVec2(0, 0));
            m_maximized = !m_maximized;
        }

        widthNeeded = buttonWidth1 + m_style->FramePadding.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        if (ImGui::Button(" X ")) {
            glfwSetWindowShouldClose(m_window, true);
        }
    }

    static void verticalSplitter() {
        float w = m_mainWindowSize.x * m_sliderRatioV; // visualiser height
        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitter", ImVec2(4.0f, -1));
        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }
        if (ImGui::IsItemActive()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            w += ImGui::GetIO().MouseDelta.x;
            m_sliderRatioV = w / m_mainWindowSize.x;
            if (m_sliderRatioV < 0.2) m_sliderRatioV = 0.2;
            if (m_sliderRatioV > 0.8) m_sliderRatioV = 0.8;
        }
    }


    static void horizontalSplitter() {
        float h = m_mainWindowSize.y * m_sliderRatioH; //text editor width
        ImGui::InvisibleButton("hsplitter", ImVec2(-1, 4.0f));
        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        }
        if (ImGui::IsItemActive()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
            h += ImGui::GetIO().MouseDelta.y;
            m_sliderRatioH = h / m_mainWindowSize.y;
            if (m_sliderRatioH < 0.3) m_sliderRatioH = 0.3;
            if (m_sliderRatioH > 0.8) m_sliderRatioH = 0.8;
        }
    }

    static void leftBar() {
        float h = m_mainWindowSize.y * m_sliderRatioH; //text editor width
        ImGui::SameLine();
        ImGui::BeginChild("side", ImVec2(0, 0));

            if (m_visualiserOpen) {
                if (!m_consoleOpen) 
                    h = 0; // if console not open and visualiser open make height full
                visualiser(ImVec2(0, h));
            }
            if (m_consoleOpen == m_visualiserOpen) { //checks if both of them are open as one of them is true if we are on this line
                horizontalSplitter();
            }
            if (m_consoleOpen) {
                consoleWindow(ImVec2(0, 0)); // fill whatever area is left
            }
        ImGui::EndChild();
    }


    static void consoleWindow(ImVec2 vec2) {
        ImGui::BeginChild("console", vec2, true, m_myChildWindowFlags);
            ImGui::BeginMenuBar();
                ImGui::Text("console");
                float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

                int widthNeeded = buttonWidth1 + m_style->FramePadding.x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                if (ImGui::Button(" X ")) {
                    m_consoleOpen = false;
                }

            ImGui::EndMenuBar();
        ImGui::EndChild();
    }




    static void textEditor(ImVec2 vec2) {
        ImGui::BeginChild("textEditor", vec2, true, m_myChildWindowFlags);
            ImGui::BeginMenuBar();
                ImGui::Text("textEditor");
            ImGui::EndMenuBar();
            ImGui::InputTextMultiline("##source", m_riscv->getTextPtr(), ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35));
        ImGui::EndChild();
    }


    static void visualiser(ImVec2 vec2) {
        ImGui::BeginChild("visualiser", vec2, true, m_myChildWindowFlags);
            ImGui::BeginMenuBar();
                ImGui::Text("visualiser");
                float buttonWidth1 = ImGui::CalcTextSize(" X ").x;

                int widthNeeded = buttonWidth1 + m_style->FramePadding.x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                if (ImGui::Button(" X ")) {
                    m_visualiserOpen = false;
                }
            ImGui::EndMenuBar();
        ImGui::EndChild();
    }

    static bool updateWorkArea() {
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &m_workXpos, &m_workYpos, &m_workWidth, &m_workHeight);
        if (ImGui::GetWindowSize().x == m_workWidth && ImGui::GetWindowSize().y == m_workHeight &&
            ImGui::GetWindowPos().x == m_workXpos && ImGui::GetWindowPos().y == m_workYpos)
            return true;
        return false;
    }
}