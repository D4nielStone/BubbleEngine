#include "ui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "iostream"
#include "string"

namespace Bubble {
	namespace Interface
	{
		UI::UI() : window_size(ImVec2(0,0)){}
		void UI::configurar(GLFWwindow* glfwWindow)
		{
			// ImGui initialization
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsDark();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
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
				ImGui::EndMainMenuBar();
			}

			for (auto j : janelas)
			{
				switch (j.first)
				{
				case Bubble::Interface::Janela::Editor:

					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
					ImGui::Begin(std::string(std::string("Editor #") + std::to_string(jnum)).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);
					window_size = ImGui::GetWindowSize();
					ImGui::Image((void*)(intptr_t)j.second, window_size, ImVec2(0, 1), ImVec2(1, 0));
					ImGui::End();
					ImGui::PopStyleVar();

					break;

				case Bubble::Interface::Janela::Preview:
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
		}
		void UI::limpar()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		void UI::novaJanela(Janela janela, GLuint framebuffer)
		{
			janelas.push_back(std::pair(janela, framebuffer));
		}
	}
}