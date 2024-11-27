#include "UI.h"
#include "Console.h"

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
        

        // load font
        m_io->Fonts->AddFontDefault();
        if (Util::fileExists(m_mainFontPath)) {
            m_mainFont = m_io->Fonts->AddFontFromFileTTF(m_mainFontPath, 22.0f);
        }
        else {
            Console::log("Failed To Load Font. Reverting To Default Font!\n Make Sure The Font File Is in The Same Folder As .exe");
            m_mainFont = m_io->Fonts->Fonts[0];
        }
        


        ImGui::StyleColorsDark();

        m_style = &ImGui::GetStyle();
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            m_style->WindowRounding = 0.0f;
            m_style->Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        m_highlightCol = ImGui::GetColorU32(ImVec4(0.35f, 0.37f, 0.37f, 1.0f));
        m_spColor = ImGui::GetColorU32(ImVec4(0.25f, 0.22f, 0.32f, 1.0f));


        ImGui_ImplGlfw_InitForOpenGL(w, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        Console::log("Init UI");
    }


    void drawUI() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        mainWindow();
        ImGui::ShowDemoWindow();
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
        ImGui::PushFont(m_mainFont);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32_BLACK_TRANS);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32_BLACK_TRANS);

        ImGui::Begin("RISCV emulator", (bool*)false, m_myMainWindowFlags);
            m_mainWindowSize = ImGui::GetWindowSize();
            mainMenuBar();


            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32_BLACK_TRANS);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            //ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32_BLACK_TRANS);

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

        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopFont();
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
                m_consoleOpen = true;
                m_riscv->run();
            }
            if (ImGui::Button(" Step ~ ")) {
                m_consoleOpen = true;
                m_riscv->step();
            }
            if (ImGui::Button(" Reset @ ")) {
                m_consoleOpen = true;
                m_riscv->reset();
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
            if (m_sliderRatioV < 0.1) m_sliderRatioV = 0.1;
            if (m_sliderRatioV > 0.9) m_sliderRatioV = 0.9;
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
            if (m_sliderRatioH < 0.1) m_sliderRatioH = 0.1;
            if (m_sliderRatioH > 0.9) m_sliderRatioH = 0.9;
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



            ImGui::InputTextMultiline("##source", Console::getStringPtr(), 
                ImVec2(ImGui::GetWindowSize().x - 15, ImGui::GetWindowSize().y - 35),
                m_ConsoleInputFlags);
            //ImGui::TextUnformatted(Console::getStartPtr(), Console::getEndPtr());

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
            drawRegisters();
            registerSplitter();
            ImGui::SameLine();
            drawMemory();




        ImGui::EndChild();
    }

    static void drawRegisters() {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("999999999999").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg  | ImGuiTableFlags_Resizable;



        


        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(m_regSliderRatio * ImGui::GetWindowSize().x, 0.0f);
        if (ImGui::BeginTable("Registers", 2, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Register");
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            const int* regPtr = m_riscv->getRegistersPtr();

            m_currRegRow = ImGui::TableGetHoveredRow();
            m_currRegCol = ImGui::TableGetHoveredColumn();


            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(32);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {


                    
                    //std::cout << row << " " << clipper.DisplayStart << " " << m_currRegRow<<std::endl;
                    ImGui::TableNextRow();
                        if(row == 2)
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_spColor, -1);


                    if (clipper.DisplayStart == 0 && m_currRegRow == 1) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, m_highlightCol, m_currRegCol);
                    }
                    if (clipper.DisplayStart != 0 && row + 2 == clipper.DisplayStart + m_currRegRow) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, m_highlightCol, m_currRegCol);
                    }
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("x%d", row);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%d", regPtr[row]);
                }
            }


            ImGui::EndTable();
        }
    }


    static void drawMemory() {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("999999999999").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;


        const char* memPtr= (char*)m_riscv->getMemoryPtr();



        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(0.0f, 0.0f);
        if (ImGui::BeginTable("Memory", 2, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            m_currMemRow = ImGui::TableGetHoveredRow();

            ImGuiListClipper clipper;
            int size = m_riscv->getMemorySize();

            int spId = size - m_riscv->readRegister(2) - 1;

            clipper.Begin(size);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    ImGui::TableNextRow();


                    if(row == spId)
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_spColor, -1);


                    if (m_selectingMem) {
                        if (clipper.DisplayStart == 0 && row < m_selecMemEnd && row >=  m_selecMemStart - 1) {
                            m_selectedMemOffset = m_riscv->getMemorySize() - m_selecMemEnd;
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_highlightCol, -1);
                        }
                        if (clipper.DisplayStart != 0 &&row + 1 < clipper.DisplayStart + m_selecMemEnd && row >= clipper.DisplayStart + m_selecMemStart - 2) {
                            m_selectedMemOffset = m_riscv->getMemorySize() - (clipper.DisplayStart + m_selecMemEnd - 1);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_highlightCol, -1);
                        }
                    }
                    else
                    {
                        if (clipper.DisplayStart == 0 && m_currMemRow == 1) {
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_highlightCol, -1);
                        }
                        if (clipper.DisplayStart != 0 && row + 2 == clipper.DisplayStart + m_currMemRow) {
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, m_highlightCol, -1);
                        }
                    }
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", size - row - 1);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%d", *(char*)(memPtr + size - row - 1));
                }
            }
            ImGui::EndTable();
        }
       
        if (m_selectingMem) {
            int num = 0;
            char* ptr = (char*)(m_riscv->getMemoryPtr()) + m_selectedMemOffset;
            std::cout << m_selectedMemOffset<<std::endl;
            std::string type = "";
            if (m_selectLen == 1) {
                type = "byte";
                num = *ptr;
            }
            if (m_selectLen == 2) {
                type = "short";
                num = *(short*)ptr;
            }
            if (m_selectLen == 4) {
                type = "int";
                num = *(int*)ptr;
            }
            ImGui::SetTooltip("%s %d", type, num);
        }
    }

    static void registerSplitter() {
        float w = ImGui::GetWindowSize().x * m_regSliderRatio; // visualiser height
        ImGui::SameLine();
        ImGui::InvisibleButton("regSplitter", ImVec2(4.0f, -1));
        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }
        if (ImGui::IsItemActive()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            w += ImGui::GetIO().MouseDelta.x;
            m_regSliderRatio = w / ImGui::GetWindowSize().x;
            if (m_regSliderRatio < 0.1) m_regSliderRatio = 0.1;
            if (m_regSliderRatio > 0.9) m_regSliderRatio = 0.9;
        }
    }

    static bool updateWorkArea() {
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &m_workXpos, &m_workYpos, &m_workWidth, &m_workHeight);
        if (ImGui::GetWindowSize().x == m_workWidth && ImGui::GetWindowSize().y == m_workHeight &&
            ImGui::GetWindowPos().x == m_workXpos && ImGui::GetWindowPos().y == m_workYpos)
            return true;
        return false;
    }

    void processInput() {
        handleMemorySelect();
        
    }

    void handleMemorySelect() {
        if (ImGui::IsMouseDown(0)) {
            if (!m_mouseWasDown) { //if first time pressed
                m_startMemRow = m_currMemRow;
                m_mouseWasDown = true;
            }
            else if (m_startMemRow != -1) {// if was already pressing and it started pressing on memory table
                m_selectingMem = true;
                m_selectLen = 1;
                //Console::log("Selecting");
                if ((m_currMemRow != -1 && m_currMemRow <= m_startMemRow) || (m_currMemRow == -1 && !m_selectUp)) {

                    m_selecMemStart = m_currMemRow;
                    m_selecMemEnd = m_startMemRow;
                    
                    if (m_selecMemStart < 0) // if out of bounds
                        m_selecMemStart = m_selecMemEnd;


                    if (m_startMemRow - m_selecMemStart >= 3) {
                        m_selecMemStart = m_startMemRow - 3;
                        m_selectLen = 4;
                    }
                    else if (m_startMemRow - m_selecMemStart >= 1) {
                        m_selecMemStart = m_startMemRow - 1;
                        m_selectLen = 2;
                    }

                    m_selecMemEnd = m_startMemRow;
                    m_selectUp = false;
                }
                else
                {
                    m_selecMemStart = m_startMemRow;
                    m_selecMemEnd = m_currMemRow;
                    if (m_selecMemEnd < 0) // if out of bounds
                        m_selecMemEnd = m_selecMemStart;

                    if (m_selecMemEnd - m_startMemRow >= 3) {
                        m_selecMemEnd = m_startMemRow + 3;
                        m_selectLen = 4;
                    }
                    else if (m_selecMemEnd - m_startMemRow >= 1) {
                        m_selecMemEnd = m_startMemRow + 1;
                        m_selectLen = 2;
                    }
                    m_selectUp = true;
                }

            }
        }
        else {
            m_selectingMem = false;
            m_mouseWasDown = false;
        }
    }
}