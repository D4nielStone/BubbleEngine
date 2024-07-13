#include "ui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/nucleo/scenemanager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "iostream"
#include "string"

namespace Bubble {
	namespace Interface
	{
		UI::UI() {}
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
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(sm->glfwWindow, true);
			ImGui_ImplOpenGL3_Init("#version 130"); // Use the appropriate GLSL version
		}
		void UI::novoFrame()
		{
			// Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			unsigned int jnum = 0;

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

			for (Janela j : janelas)
			{
				switch (j)
				{
				case Bubble::Interface::Janela::Editor:

					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
					ImGui::Begin(std::string(std::string("Editor ") + std::to_string(jnum)).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);

					engine->renderizar(Modo::Editor, ImGui::GetWindowSize());

					ImGui::Image((void*)(intptr_t)engine->gerenciadorDeCenas.cenaAtual()->camera_editor.FBO, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::End();
					ImGui::PopStyleVar();

					break;

				case Bubble::Interface::Janela::Preview:
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
					ImGui::Begin(std::string(std::string("Preview ") + std::to_string(jnum)).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);
					
					engine->renderizar(Modo::Jogo, ImGui::GetWindowSize());
					if (engine->gerenciadorDeCenas.cenaAtual()->camera_principal)
					{
						ImGui::Image((void*)(intptr_t)engine->gerenciadorDeCenas.cenaAtual()->camera_principal->FBO, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
					}
					ImGui::End();
					ImGui::PopStyleVar();
					break;
				case Bubble::Interface::Janela::Entidades:
					break;
				case Bubble::Interface::Janela::Arquivos:
					break;
				case Bubble::Interface::Janela::Inpetor:
					break;
				default:
					break;
				}
				jnum++;
			}
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