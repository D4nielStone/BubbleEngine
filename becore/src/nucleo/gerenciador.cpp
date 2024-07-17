#include <glad/glad.h>
#include "gerenciador.h"
#include "src/componentes/prototipo/terreno.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "src/arquivadores/imageloader.h"
#include "filesystem"

float deltaTime = 1;
namespace Bubble::Nucleo
{
    Engine::Engine() {}

    //@Initialize GLFW and GLAD
    bool Engine::inicializacao()
    {
        if (!glfwInit())
            return false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindow = glfwCreateWindow(800, 500, "MotorBubble-Editor", NULL, NULL);
        if (!glfwWindow)
        {
            glfwTerminate();
            return false;
        }


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;

        auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        GLFWimage icone = icone_.converterParaGlfw();
        if (icone_.carregado)
        {
            glfwSetWindowIcon(glfwWindow, 1, &icone);
        }

        glfwMakeContextCurrent(glfwWindow);
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
        glfwMakeContextCurrent(glfwWindow);
        float st = glfwGetTime();
        auto cena = gerenciadorDeCenas.cenaAtual();
        Componentes::Camera* cam = nullptr;
        if (m == Modo::Editor)
            cam = &cena->camera_editor;
        else if(cena->camera_principal)
            cam = cena->camera_principal;

        if (cam)
        {
            //fix framebuffer resolution
            glBindFramebuffer(GL_FRAMEBUFFER, cam->FBO);

            glBindTexture(GL_TEXTURE_2D, cam->textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportSize.x, viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, cam->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
        //render scene
        deltaTime = glfwGetTime() - st;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gerenciadorDeCenas.atualizarCenaAtual(m, deltaTime, static_cast<float>(viewportPos.x), static_cast<float>(viewportPos.y), static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        std::ifstream ifs("projetos.json");
        if (!ifs.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo 'projetos.json'." << std::endl;
            return 1;
        }

        // Leia o conteúdo do arquivo
        std::string jsonContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();

        // Parseie o documento JSON
        rapidjson::Document document;
        document.Parse(jsonContent.c_str());
            //if(!engine)
        //    engine = new Engine();
        //
        //engine->projeto = proj;
        //if (!engine->inicializacao())
        //    return -1;
        //
        //while (!engine->pararloop())
        //{
        //    ui.novoFrame(engine->glfwWindow, Interface::Engine);
        //    ui.renderizar();
        //}
        return true;
    }
    bool Gerenciador::salvarProjetos() {
        //salva arquivos(cenas etc.)

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
        Projeto proj{ nome, rootpath };
        Projetos.push_back(proj);

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
                return 1;
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
                const rapidjson::Value& projetos = document["projetos"];
                for (rapidjson::SizeType i = 0; i < projetos.Size(); ++i) {
                    if (projetos[i].HasMember("nome") && projetos[i]["nome"].IsString() &&
                        projetos[i].HasMember("path") && projetos[i]["path"].IsString()) {
                        std::cout << "Projeto " << i + 1 << ": " << std::endl;
                        std::cout << "  Nome: " << projetos[i]["nome"].GetString() << std::endl;
                        std::cout << "  Path: " << projetos[i]["path"].GetString() << std::endl;
                        Projetos.push_back(Projeto{ projetos[i]["nome"].GetString(), projetos[i]["path"].GetString() });
                    }
                    else {
                        std::cerr << "O projeto " << i + 1 << " não tem os campos 'nome' e 'path' corretamente definidos." << std::endl;
                        return -1;
                    }
                }
            }
            else {
                std::cerr << "O membro 'projetos' não foi encontrado ou não é um array." << std::endl;
                return -1;
            }
            Debug::emitir(Debug::Mensagem, "Projetos escaneados");
            return 1;
        }
        else
        {
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
        Interface::UI ui;
        ui.inicializarImGui(*this);
        ui.novoContexto(janelaGerenciador);
        ui.novaJanela(Interface::Projetos);
        
        //inicia glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Debug::emitir(Debug::Erro, "GLAD não inicializado");
            return false;
        }
        
        //defini o ícone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        GLFWimage icone = icone_.converterParaGlfw();
        if (icone_.carregado)
        {
            glfwSetWindowIcon(janelaGerenciador, 1, &icone);
        }

        escanearProjetos();

        //loop principal
        while (!glfwWindowShouldClose(janelaGerenciador))
        {
            ui.renderizar();
        }
        return 1;
    }
    void Gerenciador::limpar()
    {
        glfwDestroyWindow(janelaGerenciador);
        glfwTerminate();
    }
    std::vector<Projeto>* Gerenciador::obterProjetos()
    {
        return &Projetos;
    }
}