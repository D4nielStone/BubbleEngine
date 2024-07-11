#include "gerenciador.h"
#include "src/componentes/prototipo/terreno.h"
#include "src/inputs/gameinputs.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "filesystem"
#include "imgui.h"
#include "src/arquivadores/imageloader.h"

float deltaTime = 1;

Bubble::Nucleo::Gerenciador::Gerenciador(){}
Bubble::Nucleo::Gerenciador::~Gerenciador(){}


//@Initialize GLFW and GLAD
bool Bubble::Nucleo::Gerenciador::inicializacao() 
{
    if (!glfwInit())
        return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindow = glfwCreateWindow(640, 480, "Bubble", NULL, NULL);
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
    return true;
}
int Bubble::Nucleo::Gerenciador::pararloop() 
{
    return glfwWindowShouldClose(glfwWindow);
}
void Bubble::Nucleo::Gerenciador::renderizar(Modo m, ImVec2 tamanhoJanela)
{
    float st = glfwGetTime();
    if (m == Modo::Editor)
    {
        int w, h;
        glfwGetFramebufferSize(glfwWindow, &w, &h);
        // Bind to the framebuffer and render the scene
        glBindFramebuffer(GL_FRAMEBUFFER, gerenciadorDeCenas.camera_do_editor.FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gerenciadorDeCenas.atualizarCenaAtual(Modo::Editor, deltaTime, tamanhoJanela.x, tamanhoJanela.y, w, h);
        gerenciadorDeCenas.camera_do_editor.transformacao->definirPosicao(glm::vec3(125, 10, 125));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
    deltaTime = glfwGetTime() - st;
}
void Bubble::Nucleo::Gerenciador::limpar() const {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}
std::shared_ptr<Bubble::Nucleo::Scene> Bubble::Nucleo::Gerenciador::criarProjetoPadrao()
{
    auto scene = std::make_shared<Scene>("Cena Padrão");
    auto terreno = std::make_shared<Bubble::Entidades::Entidade>("Terreno");
    
    terreno->adicionarComponente(std::make_shared<Bubble::Componentes::Terreno>());
    
    scene->adicionarEntidade(terreno);
    
    gerenciadorDeCenas.adicionarCena(scene);
    gerenciadorDeCenas.carregarCena(gerenciadorDeCenas.numeroDeCenas()-1);
    return scene;
}
bool Bubble::Nucleo::Gerenciador::criarProjeto(const std::string& rootpath, const std::string& nome)
{
    rapidjson::Document doc;
    doc.SetArray();

    auto scene = criarProjetoPadrao();

    scene->serializar(&doc);

        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/modelos");
        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/texturas");
        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/shaders");
        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/sons");
        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/cenas");
        std::filesystem::create_directories(rootpath + "/" + nome + "/ativos/materiais");
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        
        std::ofstream output(rootpath + "/" + nome + "/ativos/cenas/" + scene->nome() + ".bubble");
        if (!output.is_open())
        {
            return false;
        }
        std::cout << buffer.GetString();
        output << buffer.GetString();
        output.close();
    return true;
}
bool Bubble::Nucleo::Gerenciador::carregarProjeto(const std::string& path)
{
    
    return true;
}