
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "contexto.hpp"
#include <shlobj.h> // Necessário para SHGetKnownFolderPath
#include <map>
#include <future>
#include <thread>
#include <comdef.h> // Necessário para _bstr_t (conversão de wchar_t para string)
#include "src/ui/ancoragem/ancora.hpp"
#include <functional>
#include "src/arquivadores/shader.hpp"
#include "src/arquivadores/imageloader.hpp"
#include <windows.h>
#include <memory>
#include <queue>

// contextos existentes
std::map<GLFWwindow*, std::shared_ptr<BubbleUI::Contexto>> contextos;

// Contexto padrão
BubbleUI::Contexto* contexto_atual{ nullptr };

std::queue<std::function<void()>> tarefas;

static std::string obterDiretorioDoc()
{
    // Ponteiro para armazenar o caminho
    PWSTR caminhoDocumentos = NULL;
    std::string caminhoDocumentosStr;

    // Obtém o caminho da pasta Documentos (FOLDERID_Documents)
    HRESULT resultado = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &caminhoDocumentos);

    if (SUCCEEDED(resultado)) {
        // Converte o caminho de wchar_t* para string (opcional)
        _bstr_t bstrPath(caminhoDocumentos);
        caminhoDocumentosStr = bstrPath;
    }
    else {
    }

    // Libera a memória alocada
    if (caminhoDocumentos) {
        CoTaskMemFree(caminhoDocumentos);
    }
    return caminhoDocumentosStr;
}

BubbleUI::Contexto::Contexto(GLFWwindow* window) : ancora_root(new Ancora(Horizontal))
{

    cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
    cursor_texto = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursor = cursor_normal;
    dirDoProjeto = obterDiretorioDoc() + R"(\Bubble Engine\Projetos)";
    if (window != nullptr) definirJanela(window);
}

BubbleUI::Contexto::~Contexto() = default;

static void callback_size(GLFWwindow* janela, int width, int height)
{
    contexto_atual->tamanho.height = height;
    contexto_atual->tamanho.width = width;
}

void BubbleUI::Contexto::definirJanela(GLFWwindow* janela)
{
    glfwWindow = janela;
    inputs = std::make_shared<Bubble::Inputs::Inputs>();

    // definir inputs
    glfwSetWindowUserPointer(glfwWindow, inputs.get());
    glfwSetCursorPosCallback(glfwWindow, mousePosCallBack);
    glfwSetKeyCallback(glfwWindow, callbackKey);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallBack);
    glfwSetCharCallback(glfwWindow, charCallback);
    glfwSetFramebufferSizeCallback(glfwWindow, callback_size);

    tamanho = {800, 400};
}

void BubbleUI::novoContexto(GLFWwindow* window)
{
    if (!window) abort();   // Janela inválida
    contextos[window] = std::make_shared<Contexto>(window);
    contexto_atual = contextos[window].get();
    rect_vertex.carregado = false;
    linha_vertex.carregado = false;
    halfcircle_vertex.carregado = false;
}

void BubbleUI::adicionarAncora(Contexto* contexto, BubbleUI::Ancora* ancora)
{
    if (!contexto) return;
    ancora->definirContexto(contextos[contexto->glfwWindow]);
    contexto->ancoras.emplace_back(ancora);
}

bool gatilho_ancora{ false };

void BubbleUI::Contexto::atualizar()
{

    if (!gatilho_ancora) {
        ancora_root->definirContexto(contextos[glfwWindow]);
        gatilho_ancora = true;
    }

    glfwPollEvents();

    // Vetor de futures para armazenar as tarefas assíncronas de atualização dos paineis
    std::vector<std::future<void>> futures;

    cursor = cursor_normal;

    ancora_root->bounds = {0, 0, tamanho.width, tamanho.height};
    ancora_root->atualizarBounds();
    ancora_root->atualizarPaineis();

    // Inicia cada painel->atualizar() em uma thread separada
    for (auto& ancora : ancoras) {
        futures.push_back(std::async(std::launch::async, [&ancora]() {
            ancora->atualizarBounds();
            ancora->atualizarPaineis();
          }));
    }

    for (auto& future : futures) {
        future.wait();
    }
}
void BubbleUI::Contexto::renderizar()
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, tamanho.width, tamanho.height);

    ancora_root->renderizarPaineis();
    for (const auto& ancora : ancoras)
        ancora->renderizarPaineis();

    glfwSetCursor(glfwWindow, cursor);

    glfwSwapBuffers(glfwWindow);

    /**
    * Executa as funcoes na thread principal
    */
    while (!tarefas.empty()) {
        auto &tarefa = tarefas.front();
            tarefa();
        tarefas.pop();
    }
}
void BubbleUI::atualizarContexto()
{
    if (!contexto_atual) abort(); /// Contexto inválido
    contexto_atual->atualizar();
}

void BubbleUI::renderizarContexto()
{
    if (!contexto_atual) abort(); /// Contexto inválido
    // ativa o stencil
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);

    contexto_atual->renderizar();

    glEnable(GL_CULL_FACE);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
}

std::shared_ptr<BubbleUI::Contexto> BubbleUI::janela(const char* title)
{
        // inicia glfw
        if (!glfwInit())
            return nullptr;

        GLFWwindow* shared{ NULL };

        auto janela = glfwCreateWindow(800, 400, title, NULL, shared);
        if (!janela) return nullptr;

        if(!shared)glfwMakeContextCurrent(janela);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return nullptr;
        }

        // define o ícone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("icon.ico");
        const GLFWimage icone = icone_.converterParaGlfw();

        if (icone_.carregado)   glfwSetWindowIcon(janela, 1, &icone);

        // Cria novo contexto ui

        // ativa blend
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        BubbleUI::novoContexto(janela);

        return contextos[janela];
}

bool BubbleUI::fim()
{
    if (contexto_atual)
        return glfwWindowShouldClose(contexto_atual->glfwWindow);
    else
        abort();
}

void BubbleUI::tarefa(const std::function<void()> funcao)
{
    tarefas.push(funcao);
}
