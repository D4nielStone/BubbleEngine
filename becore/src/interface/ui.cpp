#include "ui.h"
#include "ShlObj.h"
#include "src/nucleo/scenemanager.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "iostream"
#include "string"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "filesystem"
#include "src/nucleo/gerenciador.h"

namespace Bubble::Interface {
    const char* UI::desktopPath()
    {
        PWSTR path = NULL;
        HRESULT hr =
            SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
        if (SUCCEEDED(hr))
        {
            std::wstring Dpath(path);
            static std::string converted = std::filesystem::path(Dpath).string();
            return converted.c_str();
        }
    }

    void UI::novoContexto(GLFWwindow* window)
    {
        janelasGLFW.push_back(window);

        //cria novo contexto para a janela
        ImGuiContext* contexto = ImGui::CreateContext();
        contextosImGui.push_back(contexto);
        ImGui::SetCurrentContext(contexto);

        //inicia imgui para essa janela
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui_ImplGlfw_InitForOpenGL(window, true);
    }
    void UI::renderizar()
    {
        unsigned int idx = 0;
        for (const auto& window : janelasGLFW)
        {
            ImGui::SetCurrentContext(contextosImGui[idx]);
            // Start the ImGui frame
            glfwMakeContextCurrent(window);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport();

            menuBar();
            iniciarJanelas();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* beckup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(beckup_current_context);
            }
            idx++;
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    void UI::inicializarImGui(Nucleo::Gerenciador& gen)
    {

        gerenciador = &gen;
        // ImGui initialization
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    }
    void UI::iniciarJanelas()
    {
        unsigned int jnum = 0;
        for (Janela j : janelas)
        {
            switch (j)
            {
            case Interface::Projetos:
                desenharProjetos(jnum);
                break;
            case Interface::Editor:
                desenharEditor(jnum);
                break;
            case Interface::Preview:
                desenharPreview(jnum);
                break;
            case Interface::Entidades:
                desenharEntidades(jnum);
                break;
            case Interface::Arquivos:
                desenharArquivos(jnum);
                break;
            case Interface::Console:
                desenharConsole(jnum);
                break;
            case Bubble::Interface::Inpetor:
                desenharInspetor(jnum);
                break;
            }
            jnum++;
        }
    }
    void UI::novaJanela(Janela janela)
    {
        if (janela == Projetos)
            path = desktopPath();
        janelas.push_back(janela);
    }

    //Desenhar Janelas
    void UI::menuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::Button("Criar projeto"))
            {
                ImGui::OpenPopup("CriarProjeto");
            }
            if (ImGui::BeginPopup("CriarProjeto"))
            {
                ImGui::InputTextWithHint("Nome do Projeto", "NovoProjeto", nome.data(), 64);
                ImGui::InputText("Local do projeto", path.data(), 250);
                ImGui::Checkbox("Criar projeto padrao", &criarPadrao);
                if (std::filesystem::exists(path))
                {
                    if (ImGui::Button("Criar"))
                    {
                        gerenciador->criarProjeto(path.data(), nome.data(), criarPadrao);
                        gerenciador->carregarProjeto(gerenciador->obterProjetos()->back());
                        ImGui::CloseCurrentPopup();
                    }
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Escolha um local de arquivo existente");
                }
                ImGui::EndPopup();
            }
            ImGui::EndMainMenuBar();
        }
    }
    void UI::desenharComponente(Comum::Componente* componente)
    {
        if (componente && ImGui::TreeNode(std::string(componente->nome() + std::string(1, ' ')).c_str()))
        {
            if (dynamic_cast<Componentes::Camera*>(componente))
            {
                auto cam = dynamic_cast<Componentes::Camera*>(componente);
                ImGui::DragFloat("angulo de visao", &cam->FOV);
                ImGui::DragFloat("minZ", &cam->zNear);
                ImGui::DragFloat("maxZ", &cam->zFar);

                const ImVec2 size(50, 50);
                ImGui::Text("Cor do ceu");
                ImGui::SameLine();
                if (ImGui::ColorButton("Cor",
                    ImColor
                    (
                        cam->ceu[0],
                        cam->ceu[1],
                        cam->ceu[2]
                    ), 0, size
                ))
                {
                    ImGui::OpenPopup("Cor");
                }

                if (ImGui::BeginPopup("Cor"))
                {
                    ImGui::ColorPicker3("Cor", dynamic_cast<Componentes::Camera*>(componente)->ceu);
                    ImGui::EndPopup();
                }
            }

            if (dynamic_cast<Componentes::Transformacao*>(componente))
            {
                auto tra = dynamic_cast<Componentes::Transformacao*>(componente);
                glm::vec3 pos(0, 0, 0), rot(0, 0, 0), scl(0, 0, 0);
                tra->decomporMatriz(&pos, &rot, &scl);
                if (ImGui::DragFloat3("Posição", &pos[0]))
                    tra->comporMatriz(pos, rot, scl);
                if (ImGui::DragFloat3("Rotação", &rot[0]))
                    tra->comporMatriz(pos, rot, scl);
                if (ImGui::DragFloat3("Escala", &scl[0]))
                    tra->comporMatriz(pos, rot, scl);
                const char* Estados[2]{ "Estatico", "Dinamico" };
                const int itens_count = IM_ARRAYSIZE(Estados);

                ImGui::Combo("Estado", (int*)&tra->estado, Estados, itens_count);
            }
            ImGui::TreePop();
        }
    }
    void UI::desenharProjetos(unsigned int depth)
    {
        ImGui::Begin(std::string("Projetos" + std::string(depth, ' ')).c_str());
        for (const auto& projeto : *gerenciador->obterProjetos())
        {
            if (ImGui::Button(std::string(projeto.nome + ":\n" + projeto.path).c_str(), ImVec2(ImGui::GetWindowWidth() / 2, 100)))
            {
                gerenciador->carregarProjeto(projeto);
            }
        }
        ImGui::End();
    }
    void UI::desenharEntidades(unsigned int depth)
    {
    }
    void UI::desenharConsole(unsigned int depth)
    {
        ImGui::Begin(std::string("Console" + std::string(depth, ' ')).c_str());
        const auto& mensagens = Debug::obterMensagems();
        // Iterar pelas mensagens e renderizá-las
        for (const auto& mensagem : *mensagens) {
            switch (mensagem.first) {
            case Debug::Mensagem:
                ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "[ Mensagem ] %s", mensagem.second);
                break;
            case Debug::Erro:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[   ERRO   ] %s", mensagem.second);
                break;
            case Debug::Alerta:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[ ALERTA! ] %s", mensagem.second);
                break;
            default:
                ImGui::Text("%s", mensagem.second);
                break;
            }
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::End();
    }
    void UI::desenharArquivos(unsigned int depth)
    {
    }
    void UI::desenharInspetor(unsigned int depth)
    {
        ImGui::Begin((std::string("Propriedades") + std::string(depth, ' ')).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);

       // if (gerenciador->engine->gerenciadorDeCenas.cenaAtual()->entidadeSelecionada)
       // {
       //     Entidades::Entidade* ent = gerenciador->engine->gerenciadorDeCenas.cenaAtual()->entidadeSelecionada;
       //     ImGui::Checkbox("Ativado", &ent->ativado);
       //     ImGui::SameLine();
       //     ImGui::InputText("Nome", ent->nome()->data(), 125);
       //     ImGui::Spacing();
       //     for (auto& comp : ent->listaDeComponentes())
       //     {
       //         desenharComponente(comp.get());
       //     }
       // }
        ImGui::End();
    }
    void UI::desenharEditor(unsigned int depth)
    {
    
    }
    void UI::desenharPreview(unsigned int depth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin((std::string("Preview") + std::string(depth, ' ')).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);

        //if (ImGui::IsWindowFocused())
        //    gerenciador->engine->inputs.setInputMode(InputMode::Game);
        //
        //windowSize = ImGui::GetContentRegionAvail();
        //windowPos = ImGui::GetWindowPos();
        //gerenciador->engine->renderizar(Modo::Jogo, windowPos, windowSize);
        //if (gerenciador->engine->gerenciadorDeCenas.cenaAtual()->camera_principal)
        //{
        //    ImGui::Image((void*)(intptr_t)gerenciador->engine->gerenciadorDeCenas.cenaAtual()->camera_principal->FBO, windowSize, ImVec2(0, 1), ImVec2(1, 0));
        //}
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void UI::limpar()
    {
        for (const auto& contexto : contextosImGui)
        {
            ImGui::SetCurrentContext(contexto);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(contexto);
        }
        contextosImGui.clear();
        Debug::emitir(Debug::Mensagem, "UI limpo");
    }
    UI::UI() {}
    UI::~UI() { limpar(); }
}