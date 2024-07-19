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
    std::wstring UI::desktopPath()
    {
        PWSTR path = NULL;
        HRESULT hr =
            SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
        if (SUCCEEDED(hr))
        {
            std::wstring Dpath(path);
            return Dpath;
        }
        return std::wstring();
    }

    void UI::novoContexto(GLFWwindow* window, Estilo e)
    {
        for (const auto& janela : janelas)
        {
            if (janela.first.first == window)
                return;
        }
        ImGuiContext* contexto = ImGui::CreateContext();
        ImGui::SetCurrentContext(contexto);

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF("assets/fontes/noto-sans/NotoSans-Medium.ttf", 16.f, NULL, io.Fonts->GetGlyphRangesDefault());
    
        if (window)
            glfwMakeContextCurrent(window);

        std::vector<Janela>* configuracao_jnl = new std::vector<Janela>();
        switch (e)
        {
        case Interface::Engine:
            configuracao_jnl->push_back(Console);
            configuracao_jnl->push_back(Editor);
            configuracao_jnl->push_back(Preview);
            configuracao_jnl->push_back(Inpetor);
            configuracao_jnl->push_back(Entidades);
            configuracao_jnl->push_back(Arquivos);
            break;
        case Interface::Vazio:
            configuracao_jnl->push_back(MENU_CriarProjeto);
            configuracao_jnl->push_back(Projetos);
            break;
        default:
            break;
        }
        janelas.push_back(std::make_pair(std::make_pair(window, contexto), configuracao_jnl));
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(window, true);
    }
    void UI::pullevents()
    {
        
    }
    void UI::renderizar()
    {
        if (janelas.empty())
        {
            gameloop = false;
            return;
        }
        for (auto& window : janelas)
        {
            auto& [glfw_window, imgui_context] = window.first;
            auto& window_config = window.second;
            if (!glfw_window)
                return;
            glfwMakeContextCurrent(glfw_window);
            glfwPollEvents();
            if (gerenciador->engineAtual)
                gerenciador->engineAtual->renderizar(Modo::Editor, ImVec2(0, 0), ImVec2(200, 200));
            if (glfwGetWindowAttrib(glfw_window, GLFW_FOCUSED))
            {
                
                ImGui::SetCurrentContext(imgui_context);

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                ImGui::PushFont(ImGui::GetFont());
                ImGui::DockSpaceOverViewport();
                iniciarJanelas(window);
                ImGui::PopFont();

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }
            glfwSwapBuffers(glfw_window);
            if (glfwWindowShouldClose(glfw_window))
            {
                limpar(window);
                return;
            }
        }
    }

    void UI::inicializarImGui(Nucleo::Gerenciador& gen)
    {
        gerenciador = &gen;
        IMGUI_CHECKVERSION();
    }

    void UI::iniciarJanelas(std::pair<std::pair<GLFWwindow*, ImGuiContext*>, std::vector<Janela>*> window)
    {
        unsigned int jnum = 0;
        for (Janela j : *window.second)
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
            case Bubble::Interface::MENU_Arquivos:
                desenharMenuArquivos();
                break;
            case Bubble::Interface::MENU_Cena:
                desenharMenuCena();
                break;
            case Bubble::Interface::MENU_Editar:
                desenharMenuEditar();
                break;
            case Bubble::Interface::MENU_CriarProjeto:
                desenharMenuCriarP();
                break;
            }
            jnum++;
        }
    }

    void UI::novaJanela(GLFWwindow* w, Janela janela)
    {
        if (janela == MENU_CriarProjeto) {
            for (size_t i = 0; i < sizeof(pathBuffer); i++)
            {
                pathBuffer[i] = std::filesystem::path(desktopPath()).string().data()[i];
            }
        }
        for (auto& jnl : janelas)
        {
            if (jnl.first.first == w)
            {
                jnl.second->push_back(janela);
            }
        }
    }

    void UI::desenharMenuCena() {}

    void UI::desenharMenuEditar() {}

    void UI::desenharMenuCriarP()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::Button("Criar projeto"))
            {
                ImGui::OpenPopup("CriarProjeto");
            }
            if (ImGui::BeginPopup("CriarProjeto"))
            {
                ImGui::InputTextWithHint("Nome do Projeto", "NovoProjeto", nomeBuffer, sizeof(nomeBuffer));
                ImGui::InputText("Local do projeto", pathBuffer, sizeof(pathBuffer));
                ImGui::Checkbox("Criar projeto padrao", &criarPadrao);
                if (std::filesystem::exists(std::filesystem::path(pathBuffer)))
                {
                    if (ImGui::Button("Criar"))
                    {
                        gerenciador->criarProjeto(std::string(pathBuffer), std::string(nomeBuffer), criarPadrao);
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

    void UI::desenharMenuArquivos() {}

    void UI::desenharComponente(Comum::Componente* componente)
    {
        if (componente && ImGui::TreeNode(std::string(componente->nome() + std::string(1, ' ')).c_str()))
        {
            if (auto cam = dynamic_cast<Componentes::Camera*>(componente))
            {
                ImGui::DragFloat("angulo de visao", &cam->FOV);
                ImGui::DragFloat("minZ", &cam->zNear);
                ImGui::DragFloat("maxZ", &cam->zFar);

                const ImVec2 size(50, 50);
                ImGui::Text("Cor do ceu");
                ImGui::SameLine();
                if (ImGui::ColorButton("Cor",
                    ImColor(cam->ceu[0], cam->ceu[1], cam->ceu[2]), 0, size))
                {
                    ImGui::OpenPopup("Cor");
                }

                if (ImGui::BeginPopup("Cor"))
                {
                    ImGui::ColorPicker3("Cor", cam->ceu);
                    ImGui::EndPopup();
                }
            }

            if (auto tra = dynamic_cast<Componentes::Transformacao*>(componente))
            {
                glm::vec3 pos(0, 0, 0), rot(0, 0, 0), scl(0, 0, 0);
                tra->decomporMatriz(&pos, &rot, &scl);
                if (ImGui::DragFloat3("Posição", &pos[0]))
                    tra->comporMatriz(pos, rot, scl);
                if (ImGui::DragFloat3("Rotação", &rot[0]))
                    tra->comporMatriz(pos, rot, scl);
                if (ImGui::DragFloat3("Escala", &scl[0]))
                    tra->comporMatriz(pos, rot, scl);
            }

            //if (auto cod = dynamic_cast<Componentes::Codigo*>(componente))
            //{
            //    ImGui::Text(std::string("Script " + cod->caminhoScript()).c_str());
            //}

            ImGui::TreePop();
        }
    }

    void UI::limpar(std::pair<std::pair<GLFWwindow*, ImGuiContext*>, std::vector<Janela>*> window)
    {
        if (window.first.first)
        {
            glfwDestroyWindow(window.first.first);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(window.first.second);
            delete window.second;
            auto it = std::find(janelas.begin(), janelas.end(), window);
            janelas.erase(it);
            Debug::emitir(Debug::Alerta, "Janela destruída");
        }
    }
    void UI::desenharProjetos(unsigned int depth)
    {
        bool carregar_projeto = false;
        Nucleo::Projeto projet;
        ImGui::Begin(std::string("Projetos" + std::string(depth, ' ')).c_str());
        for (const auto& projeto : *gerenciador->obterProjetos())
        {
            if (ImGui::Button(std::string(projeto.nome + ":\n" + projeto.path).c_str(), ImVec2(ImGui::GetWindowWidth() / 2, 100)))
            {
                projet = projeto;
                carregar_projeto = true;
            }
        }
        ImGui::End();
        if (carregar_projeto)
        {
            gerenciador->carregarProjeto(projet);
        }
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
                ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "[ Mensagem ]");
                break;
           case Debug::Erro:
               ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[   ERRO   ]");
                break;
           case Debug::Alerta:
               ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[ ALERTA! ]");
                break;
            default:
                break;
            }
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mensagem.second);
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

         if (gerenciador->engineAtual->obterGC()->cenaAtual()->entidadeSelecionada)
         {
             Entidades::Entidade* ent = gerenciador->engineAtual->obterGC()->cenaAtual()->entidadeSelecionada;
             ImGui::Checkbox("Ativado", &ent->ativado);
             ImGui::SameLine();
             ImGui::InputText("Nome", ent->nome()->data(), 125);
             ImGui::Spacing();
             for (auto& comp : ent->listaDeComponentes())
             {
                 desenharComponente(comp.get());
             }
         }
        ImGui::End();
    }
    void UI::desenharEditor(unsigned int depth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin((std::string("Editor") + std::string(depth, ' ')).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);
        Nucleo::Engine* engine = gerenciador->engineAtual;
        if (ImGui::IsWindowFocused())
            engine->obterGI()->setInputMode(InputMode::Editor);
        
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 windowPos = ImGui::GetWindowPos();
    
        ImGui::Image((void*)(intptr_t)0, windowSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();
    }
    void UI::desenharPreview(unsigned int depth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin((std::string("Preview") + std::string(depth, ' ')).c_str(), (bool*)0, ImGuiWindowFlags_NoScrollbar);
        Nucleo::Engine* engine = gerenciador->engineAtual;
        if (ImGui::IsWindowFocused())
           engine->obterGI()->setInputMode(InputMode::Game);
        
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 windowPos = ImGui::GetWindowPos();
        engine->renderizar(Modo::Jogo, windowPos, windowSize);
        if (engine->obterGC()->cenaAtual()->camera_principal)
        {
            ImGui::Image((void*)(intptr_t)engine->obterGC()->cenaAtual()->camera_principal->FBO, windowSize, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
