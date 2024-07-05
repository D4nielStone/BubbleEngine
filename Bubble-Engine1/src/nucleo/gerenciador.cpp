#include "gerenciador.h"
#include "src/componentes/transformacao/transformacao.h"
#include "src/componentes/renderizador/renderizador.h"
#include "src/inputs/gameinputs.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "filesystem"
#include <FreeImage.h>

float deltaTime = 1;

GLFWimage loadImage(const std::string& filepath)
{
    GLFWimage image = {};
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filepath.c_str(), 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(filepath.c_str());
    }
    if (format == FIF_UNKNOWN) {
        std::cerr << "Failed to determine image format: " << filepath << std::endl;
        return image;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, filepath.c_str());
    if (!bitmap) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return image;
    }

    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    image.width = FreeImage_GetWidth(converted);
    image.height = FreeImage_GetHeight(converted);
    image.pixels = new unsigned char[4 * image.width * image.height];

    unsigned char* bits = FreeImage_GetBits(converted);
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            unsigned char b = bits[4 * (y * image.width + x) + 0];
            unsigned char g = bits[4 * (y * image.width + x) + 1];
            unsigned char r = bits[4 * (y * image.width + x) + 2];
            unsigned char a = bits[4 * (y * image.width + x) + 3];

            image.pixels[4 * (y * image.width + x) + 0] = r;
            image.pixels[4 * (y * image.width + x) + 1] = g;
            image.pixels[4 * (y * image.width + x) + 2] = b;
            image.pixels[4 * (y * image.width + x) + 3] = a;
        }
    }

    FreeImage_Unload(converted);
    return image;
}
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
        //auto icone_ = Bubble::Arquivadores::ImageLoader("ICON.ico");
        //GLFWimage icone;
        //icone.pixels = icone_.obterDados();
    GLFWimage icone = loadImage("ICON.ico");
    glfwSetWindowIcon(glfwWindow, 1, &icone);
    return true;
}
int Bubble::Nucleo::Gerenciador::pararloop() 
{
    return glfwWindowShouldClose(glfwWindow);
}
//@Render OpenGL (virtual function)
void Bubble::Nucleo::Gerenciador::renderizar()
{
    /* Render here */
    float st = glfwGetTime();
    int display_w, display_h;
    glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    gerenciadorDeCenas.atualizarCenaAtual(deltaTime);
    /* Swap front and back buffers */
    glfwSwapBuffers(glfwWindow);

    /* Poll for and process events */
    glfwPollEvents();

    deltaTime = glfwGetTime() - st;
}
void Bubble::Nucleo::Gerenciador::limpar() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}
std::shared_ptr<Bubble::Nucleo::Scene> Bubble::Nucleo::Gerenciador::criarProjetoPadrao()
{
    auto scene = std::make_shared<Scene>("Cena Padrão");
    auto bola = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));
    auto chao = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/cube.dae"));
    auto cam = std::make_shared<Bubble::Entidades::Entidade>();
    
    chao->obterTransformacao()->definirPosicao(glm::vec3(0, -2, 0));
    chao->obterTransformacao()->definirEscala(glm::vec3(2, 0.5, 2));
    cam->obterTransformacao()->definirPosicao(glm::vec3(0, 0, -5));
    
    cam->adicionarComponente(std::make_shared < Bubble::Componentes::Camera>());
    bola->adicionarComponente(std::make_shared<Bubble::Componentes::Codigo>("assets/scripts/rotacionar.lua"));
    
    scene->adicionarEntidade(cam);
    scene->adicionarEntidade(chao);
    scene->adicionarEntidade(bola);
    
    gerenciadorDeCenas.adicionarCena(scene);
    gerenciadorDeCenas.carregarCena(gerenciadorDeCenas.cenaAtual());
    return scene;
}
bool Bubble::Nucleo::Gerenciador::criarProjeto(const std::string& rootpath, const std::string& nome)
{
    rapidjson::Document doc;
    doc.SetArray();

    auto scene = criarProjetoPadrao();

    scene->serializar(&doc);

//    if (!std::filesystem::exists(rootpath + "/" + nome + "/ativos"))
  //  {
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
    //}


    //else
        //return false;
    return true;
}
bool Bubble::Nucleo::Gerenciador::carregarProjeto(const std::string& path)
{
    
    return true;
}