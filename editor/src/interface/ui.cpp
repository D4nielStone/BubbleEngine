#include "ui.h"
#include "src/nucleo/scenemanager.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "iostream"
#include "string"

namespace Bubble {
	namespace Interface
	{
		UI::UI() {}
        void UI::desenharFrustums(std::vector<std::shared_ptr<Comum::Componente>> cameras)
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            for (auto& camera : cameras)
            {
                if (!camera) continue; // Verifica se a câmera é válida

                glm::mat4 viewMatrix = dynamic_cast<Componentes::Camera*>(camera.get())->obterViewMatrixMat();
                glm::mat4 projectionMatrix = dynamic_cast<Componentes::Camera*>(camera.get())->obterProjMatrixMat();
                glm::mat4 viewProjMatrix = projectionMatrix * viewMatrix;

                glm::vec3 frustumCorners[8];

                // Calculate frustum corners
                for (int i = 0; i < 8; ++i) {
                    glm::vec4 corner = glm::inverse(viewProjMatrix) * glm::vec4(
                        (i & 1) ? 1.0f : -1.0f,
                        (i & 2) ? 1.0f : -1.0f,
                        (i & 4) ? 1.0f : -1.0f,
                        1.0f
                    );
                    frustumCorners[i] = glm::vec3(corner / corner.w);
                }

                // Draw lines between frustum corners
                for (int i = 0; i < 4; ++i) {
                    drawList->AddLine(
                        ImVec2(frustumCorners[i].x, frustumCorners[i].y),
                        ImVec2(frustumCorners[(i + 1) % 4].x, frustumCorners[(i + 1) % 4].y),
                        IM_COL32(255, 255, 255, 255)
                    );
                    drawList->AddLine(
                        ImVec2(frustumCorners[i + 4].x, frustumCorners[i + 4].y),
                        ImVec2(frustumCorners[(i + 1) % 4 + 4].x, frustumCorners[(i + 1) % 4 + 4].y),
                        IM_COL32(255, 255, 255, 255)
                    );
                    drawList->AddLine(
                        ImVec2(frustumCorners[i].x, frustumCorners[i].y),
                        ImVec2(frustumCorners[i + 4].x, frustumCorners[i + 4].y),
                        IM_COL32(255, 255, 255, 255)
                    );
                }
            }
        }
		void UI::configurar(Nucleo::Engine* sm)
		{
			engine = sm;
			// ImGui initialization
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsDark();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(sm->glfwWindow, true);
			ImGui_ImplOpenGL3_Init("#version 130"); // Use the appropriate GLSL version
		}

		void UI::iniciarJanelas()
		{
            unsigned int jnum = 0;
            for (Janela j : janelas)
            {
                const float* viewMatrix = nullptr;
                const float* projectionMatrix = nullptr;
                float* objectMatrix = nullptr;
                float windowWidth = 0;
                float windowHeight = 0;
                ImVec2 windowSize(0, 0);
                ImDrawList* drawList;
                switch (j)
                {
                case Bubble::Interface::Janela::Editor:
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                    ImGui::Begin(std::string("Editor " + std::to_string(jnum)).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);

                    if (ImGui::IsWindowFocused())
                        engine->inputs.setInputMode(InputMode::Editor);
                    
                    windowSize = ImGui::GetContentRegionAvail();
                    engine->renderizar(Modo::Editor, windowSize);
                    ImGui::Image((void*)(intptr_t)engine->gerenciadorDeCenas.cenaAtual()->camera_editor.FBO, windowSize, ImVec2(0, 1), ImVec2(1, 0));

                    //ImGuizmo
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    windowWidth = ImGui::GetWindowWidth();
                    windowHeight = ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                    viewMatrix = engine->gerenciadorDeCenas.cenaAtual()->camera_editor.obterViewMatrix();
                    projectionMatrix = engine->gerenciadorDeCenas.cenaAtual()->camera_editor.obterProjMatrix();
                    
                    objectMatrix;
                    objectMatrix = engine->gerenciadorDeCenas.cenaAtual()->Entidades[1]->obterTransformacao()->obterMatrizGlobal();
                    ImGuizmo::Manipulate(viewMatrix, projectionMatrix, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, objectMatrix);

                    for (auto& entidade : engine->gerenciadorDeCenas.cenaAtual()->Entidades)
                    {
                        desenharFrustums(entidade->obterComponentes("Camera"));
                    }
                    ImGui::End();
                    ImGui::PopStyleVar();
                    break;

                case Bubble::Interface::Janela::Preview:
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                    
                    ImGui::Begin(std::string("Preview" + std::to_string(jnum)).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);
                    
                    if (ImGui::IsWindowFocused())
                        engine->inputs.setInputMode(InputMode::Game);

                    windowSize = ImGui::GetWindowSize();
                    engine->renderizar(Modo::Jogo, windowSize);
                    if (engine->gerenciadorDeCenas.cenaAtual()->camera_principal)
                    {
                        ImGui::Image((void*)(intptr_t)engine->gerenciadorDeCenas.cenaAtual()->camera_principal->FBO, windowSize, ImVec2(0, 1), ImVec2(1, 0));
                    }
                    ImGui::End();
                    ImGui::PopStyleVar();
                    break;
                case Bubble::Interface::Janela::Entidades:
                    // Handle Entidades window here
                    break;
                case Bubble::Interface::Janela::Arquivos:
                    // Handle Arquivos window here
                    break;
                case Bubble::Interface::Janela::Inpetor:
                    // Handle Inpetor window here
                    break;
                default:
                    break;
                }
                jnum++;
            }
		}
        void UI::novoFrame()
        {
            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport();
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Arquivo"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Editar"))
                {
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            
            iniciarJanelas();

        }
        void UI::renderizar()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* beckup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(beckup_current_context);
			}
			glfwSwapBuffers(engine->glfwWindow);
			glfwPollEvents();
		}
		void UI::limpar()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		void UI::novaJanela(Janela janela)
		{
			janelas.push_back(janela);
		}
	}
}