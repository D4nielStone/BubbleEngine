#include <glad/glad.h>
#include "gerenciador.h"
#include "src/componentes/prototipo/terreno.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "src/arquivadores/imageloader.h"
#include "filesystem"
#include "imgui.h"

float deltaTime = 1;
namespace Bubble::Nucleo
{
    Engine::Engine() {}
    Engine::~Engine() {}


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

        glfwMakeContextCurrent(glfwWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;

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
    void Engine::renderizar(Modo m, ImVec2 tamanhoJanela)
    {
        float st = glfwGetTime();
        int w, h;
        auto cena = gerenciadorDeCenas.cenaAtual();
        glfwGetFramebufferSize(glfwWindow, &w, &h);
        if (m == Modo::Editor)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, cena->camera_editor.FBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gerenciadorDeCenas.atualizarCenaAtual(Modo::Editor, deltaTime, w, h, tamanhoJanela.x, tamanhoJanela.y);
        }
        else
        {
            if (cena->camera_principal)
                glBindFramebuffer(GL_FRAMEBUFFER, cena->camera_principal->FBO);
            else
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gerenciadorDeCenas.atualizarCenaAtual(Modo::Editor, deltaTime, w, h, tamanhoJanela.x, tamanhoJanela.y);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        deltaTime = glfwGetTime() - st;
    }
    void Engine::limpar() const {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }
    std::shared_ptr<Bubble::Nucleo::Scene> Engine::criarProjetoPadrao()
    {
        //Cria cena
        auto scene = std::make_shared<Scene>("Cena Padrão");
        //Cira e configura entidade Terreno
        auto terreno = std::make_shared<Bubble::Entidades::Entidade>("Terreno");
        terreno->adicionarComponente(std::make_shared<Bubble::Componentes::Terreno>());

        //Cria e configura entidade Camera
        //auto camera = std::make_shared<Bubble::Entidades::Entidade>("Cam");
        //camera->adicionarComponente(std::make_shared<Bubble::Componentes::Camera>());

        auto esfera = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));

        scene->adicionarEntidade(esfera);
        scene->adicionarEntidade(terreno);
        //scene->adicionarEntidade(camera);

        gerenciadorDeCenas.adicionarCena(scene);
        gerenciadorDeCenas.carregarCena(gerenciadorDeCenas.numeroDeCenas() - 1);
        gerenciadorDeCenas.cenaAtual()->camera_editor.transformacao->Move(10, 10, -10);
        gerenciadorDeCenas.cenaAtual()->camera_editor.inputs = &inputs;

        return scene;
    }
    bool Engine::criarProjeto(const std::string& rootpath, const std::string& nome)
    {
        rapidjson::Document doc;
        doc.SetArray();
        auto scene = criarProjetoPadrao();
        scene->serializar(&doc);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        std::string fullpath = rootpath + "/" + nome;
        std::string cenaPath = fullpath + "/Cenas";

        if (std::filesystem::exists(fullpath))
        {
            std::filesystem::remove_all(fullpath);
            Debug::emitir(Debug::Tipo::Alerta, "Diretorio do projeto antigo removido");
        }
        if (!std::filesystem::create_directories(cenaPath))
        {
            Debug::emitir(Debug::Tipo::Erro, "Diretorio do projeto nao foi criado");
            return false;
        }

        std::ofstream output(cenaPath + scene->nome() + ".bubble");
        if (!output.is_open())
        {
            return false;
        }
        output << buffer.GetString();
        output.close();
        return true;
    }
    bool Engine::carregarProjeto(const std::string& path)
    {

        return true;
    }
}