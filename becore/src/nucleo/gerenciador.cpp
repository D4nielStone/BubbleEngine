#include <glad/glad.h>
#include "gerenciador.h"
#include "src/componentes/prototipo/terreno.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"
#include "src/arquivadores/imageloader.h"
#include "filesystem"

float deltaTime = 1;
namespace Bubble::Nucleo
{
    Engine::Engine() {}

    //@Initialize GLFW and GLAD
    bool Engine::inicializacao()
    {
        //inicia glfw
        if (!glfwInit())
        {
            Debug::emitir(Debug::Erro, "GLFW não inicializado");
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindow = glfwCreateWindow(800, 500, "MotorBubble-Editor", NULL, NULL);
        glfwMakeContextCurrent(glfwWindow);
        if (!glfwWindow)
        {
            Debug::emitir(Debug::Erro, "Janela não inicializada");
            glfwTerminate();
            return false;
        }
        //inicia glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Debug::emitir(Debug::Erro, "GLAD não inicializado");
            return false;
        }
        //define o ícone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        GLFWimage icone = icone_.converterParaGlfw();
        if (icone_.carregado)
        {
            glfwSetWindowIcon(glfwWindow, 1, &icone);
        }
        glfwSetWindowUserPointer(glfwWindow, &inputs);
        glfwSetKeyCallback(glfwWindow, keyCallback);

        return true;
    }
    int Engine::pararloop() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }
    void Engine::renderizar(Modo m, ImVec2 viewportPos, ImVec2 viewportSize)
    {
        float st = glfwGetTime();
        auto cena = gerenciadorDeCenas.cenaAtual();
        Componentes::Camera* cam = nullptr;

        if (m == Modo::Editor)
            cam = &cena->camera_editor;
        else if (cena->camera_principal)
            cam = cena->camera_principal;

        if (cam)
        {
            // Bind framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, cam->FBO);

            // Redimensionar o texture color buffer
            glBindTexture(GL_TEXTURE_2D, cam->textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportSize.x, viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            // Redimensionar o renderbuffer
            glBindRenderbuffer(GL_RENDERBUFFER, cam->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // Limpar framebuffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Atualizar cena
            gerenciadorDeCenas.atualizarCenaAtual(m, deltaTime, static_cast<float>(viewportPos.x), static_cast<float>(viewportPos.y), static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y));

            // Desligar framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Calcular deltaTime
        deltaTime = glfwGetTime() - st;
    }

    void Engine::limpar() const {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }
    bool Engine::salvarCena(unsigned int idx)
    {
        return true;
    }

    //GERENCIADOR
    std::vector<Projeto> Projetos;

    Scene Gerenciador::criarCenaPadrao()
    {
        //Cria cena
        Scene scene = Scene("Cena1");
        //Cira e configura entidade Terreno
        auto terreno = std::make_shared<Bubble::Entidades::Entidade>("Terreno");
        terreno->adicionarComponente(std::make_shared<Bubble::Componentes::Terreno>());

        //Cria e configura entidade Camera
        auto camera = std::make_shared<Bubble::Entidades::Entidade>("Cam");
        camera->adicionarComponente(std::make_shared<Bubble::Componentes::Camera>());
        camera->obterTransformacao()->definirPosicao(glm::vec3(10, 10, 10));
        //Cria e configura entidade Esfera
        auto esfera = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));

        scene.adicionarEntidade(esfera);
        scene.adicionarEntidade(camera);
        scene.adicionarEntidade(terreno);

        return scene;
    }
    bool Gerenciador::carregarProjeto(Projeto proj)
    {
        if (!std::filesystem::exists(proj.path))
        {
            Debug::emitir(Debug::Erro, "Não foi possível carregar o projeto: " + proj.nome);
            return false;
        }

        std::vector<std::filesystem::path> sceneFiles;
        std::vector<std::shared_ptr<Scene>> cenas;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(proj.path))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".bs")
            {
                sceneFiles.push_back(entry.path());
                Debug::emitir("CENA", "Arquivo de cena encontrado : " + entry.path().string());
            }
        }

        if (sceneFiles.empty())
        {
            Debug::emitir(Debug::Mensagem, "Nenhum arquivo de cena encontrado no projeto: " + proj.nome);
        }
        else
        {
            Debug::emitir(Debug::Mensagem, "Arquivos de cena carregados para o projeto: " + proj.nome);
            for (const auto& sceneFile : sceneFiles)
            {
                // Carregangdo o arquivo de cena para a classe
                std::ifstream ifs(sceneFile);
                if (!ifs.is_open()) {
                    Debug::emitir(Debug::Erro, "Não foi possível abrir o arquivo de cena: " + sceneFile.filename().string());
                    return false;
                }

                // Leia o conteúdo do arquivo
                std::string jsonContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
                ifs.close();

                // Parseie o documento JSON
                rapidjson::Document document;
                document.Parse(jsonContent.c_str());

                // parse da cena
                std::shared_ptr<Scene> cenatmp = std::make_shared<Scene>();
                if (cenatmp->parse(document))
                    cenas.push_back(cenatmp);
                else {
                    Debug::emitir(Debug::Erro, "Parse da cena");
                    return false;
                }
            }
        }

        // Adiciona nova engine para o projeto
        Engine* eng = new Engine();
        eng->obterProjeto()->nome = proj.nome;
        eng->obterProjeto()->path = proj.path;
        for (const auto& cena: cenas)
        {
            eng->obterGC()->adicionarCena(cena);
        }
        eng->obterGC()->carregarCena(0);
        eng->inicializacao();
        engines.push_back(eng);

        return true;
    }

    bool Gerenciador::salvarListaProjetos() {
        //salvando lista de projetos
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        rapidjson::Value projetos(rapidjson::kArrayType);
        for (const auto& projeto : Projetos) {
            rapidjson::Value projetoObj(rapidjson::kObjectType);
            projetoObj.AddMember("nome", rapidjson::Value().SetString(projeto.nome.c_str(), allocator), allocator);
            projetoObj.AddMember("path", rapidjson::Value().SetString(projeto.path.c_str(), allocator), allocator);
            projetos.PushBack(projetoObj, allocator);
        }

        document.AddMember("projetos", projetos, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        std::ofstream ofs("projetos.json");
        if (!ofs.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo 'projetos.json' para escrita." << std::endl;
            return false;
        }
        ofs << buffer.GetString();
        ofs.close();

        Debug::emitir(Debug::Mensagem, "Projetos salvos");
        return true;
    }
    bool Gerenciador::criarProjeto(const std::string& rootpath, const std::string& nome, bool criarCenaPadra)
    {
        Projeto proj{ nome, rootpath + "/" + nome};
        Projetos.push_back(proj);
        salvarListaProjetos();

        // Criar cena
        Scene cena("Cena1");
        if (criarCenaPadra)
            cena = criarCenaPadrao();

        // Serializar cena
        rapidjson::Document doc;
        cena.serializar(&doc);

        // Salvar no buffer
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        // Cria diretório caso não exista
        std::string scenePath = rootpath + "/" + nome + "/Cenas";
        if (!std::filesystem::exists(scenePath))
        {
            std::filesystem::create_directories(scenePath);
        }

        // Salva cena no arquivo
        std::string cenaFilePath = scenePath + "/" + cena.nome() + ".bs";
        std::ofstream cenaFile(cenaFilePath);
        cenaFile << buffer.GetString();
        cenaFile.close();

        // Carrega o projeto
        carregarProjeto(proj);
        Debug::emitir(Debug::Mensagem, "Cena padrão criada");

        return true;
    }

    bool Gerenciador::escanearProjetos()
    {
        if (std::filesystem::exists("projetos.json"))
        {
            std::ifstream ifs("projetos.json");
            if (!ifs.is_open()) {
                std::cerr << "Não foi possível abrir o arquivo 'projetos.json'." << std::endl;
                return false;
            }

            // Leia o conteúdo do arquivo
            std::string jsonContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            ifs.close();

            // Parseie o documento JSON
            rapidjson::Document document;
            document.Parse(jsonContent.c_str());

            // Verifique e leia o array de projetos
            if (document.HasMember("projetos") && document["projetos"].IsArray())
            {
                rapidjson::Value& projetos = document["projetos"];
                rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
                for (rapidjson::SizeType i = 0; i < projetos.Size();)
                {
                    if (projetos[i].HasMember("nome") && projetos[i]["nome"].IsString() &&
                        projetos[i].HasMember("path") && projetos[i]["path"].IsString())
                    {
                        if (std::filesystem::exists(projetos[i]["path"].GetString()))
                        {
                            Debug::emitir("PROJETO", "Projeto " + std::to_string(i + 1) + ":");
                            Debug::emitir("PROJETO", "  Nome: " + std::string(projetos[i]["nome"].GetString()));
                            Debug::emitir("PROJETO", "  Path: " + std::string(projetos[i]["path"].GetString()));
                            Projetos.push_back(Projeto{ projetos[i]["nome"].GetString(), projetos[i]["path"].GetString() });
                        }
                        else
                        {
                            Debug::emitir(Debug::Erro, "O diretório do projeto " + std::string(projetos[i]["nome"].GetString()) + " não existe");
                            projetos.Erase(projetos.Begin() + i);
                        }
                        ++i;
                    }
                    else
                    {
                        Debug::emitir(Debug::Erro, "O projeto " + std::to_string(i + 1) + " não tem os campos 'nome' e 'path' corretamente definidos.");
                        return false;
                    }
                }
            }
            else
            {
                Debug::emitir(Debug::Erro, "O membro 'projetos' não foi encontrado ou não é um array");
                return false;
            }

            // Escreve o documento JSON de volta ao arquivo
            std::ofstream ofs("projetos.json");
            if (!ofs.is_open()) {
                std::cerr << "Não foi possível abrir o arquivo 'projetos.json' para escrita." << std::endl;
                return false;
            }
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            document.Accept(writer);

            Debug::emitir(Debug::Mensagem, "Projetos escaneados e atualizados");
            return true;
        }
        else
        {
            std::ofstream ofs("projetos.json");
            if (!ofs.is_open()) {
                std::cerr << "Não foi possível abrir o arquivo 'projetos.json' para escrita." << std::endl;
                return false;
            }
            Debug::emitir(Debug::Mensagem, "Nenhum projeto existente");
        }
        return true;
    }

    bool Gerenciador::inicializacao()
    {
        //inicia glfw
        if (!glfwInit())
        {
            Debug::emitir(Debug::Erro, "GLFW não inicializado");
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        janelaGerenciador = glfwCreateWindow(800, 500, "MotorBubble-Gerenciador de Projetos", NULL, NULL);
        glfwMakeContextCurrent(janelaGerenciador);
        if (!janelaGerenciador)
        {
            Debug::emitir(Debug::Erro, "Janela não inicializada");
            glfwTerminate();
            return false;
        }

        //inicia UI
        ui.inicializarImGui(*this);
        ui.novoContexto(janelaGerenciador);
        
        //inicia glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Debug::emitir(Debug::Erro, "GLAD não inicializado");
            return false;
        }
        
        //define o ícone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        GLFWimage icone = icone_.converterParaGlfw();
        if (icone_.carregado)
        {
            glfwSetWindowIcon(janelaGerenciador, 1, &icone);
        }

        escanearProjetos();

        //loop principal
        while (ui.gameloop)
        {
            ui.pullevents();
            for (const auto& engine : engines)
            {
                engineAtual = engine;
                ui.novoContexto(engine->obterJanela(), Interface::Engine);
                auto it = std::find(engines.begin(), engines.end(), engine);
                if (it != engines.end())
                {
                    engines.erase(it);
                }
            }
            ui.renderizar();
        }
        return 1;
    }
    void Gerenciador::limpar()
    {
        glfwTerminate();
    }
    std::vector<Projeto>* Gerenciador::obterProjetos()
    {
        return &Projetos;
    }
}