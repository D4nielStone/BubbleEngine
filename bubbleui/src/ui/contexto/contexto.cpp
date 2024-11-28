
// Copyright (c) 2024 Daniel Oliveira

#include "contexto.hpp"
#include <shlobj.h> // Necess�rio para SHGetKnownFolderPath
#include <map>
#include <future>
#include <thread>
#include <comdef.h> // Necess�rio para _bstr_t (convers�o de wchar_t para string)
#include "src/ui/painel/painel.hpp"
#include <functional>
#include "src/arquivadores/shader.hpp"
#include "src/arquivadores/imageloader.hpp"
#include "src/ui/painel/editor.hpp"
#include "src/ui/painel/depurador.hpp"
#include <windows.h>
#include <memory>
#include <queue>

// contextos existentes
std::map<GLFWwindow*, std::shared_ptr<BubbleUI::Contexto>> contextos;

// Contexto padr�o
BubbleUI::Contexto* contexto_atual{ nullptr };

std::queue<std::function<void()>> tarefas;

std::shared_ptr<BubbleUI::Painel> BubbleUI::Contexto::painel(const char* nome_painel, const Vector4& rect)
{
    std::shared_ptr<Painel> _Pnl_tmp = std::make_shared<Painel>(); // Shared ownership for temporary panel
    tarefa([this, rect, nome_painel, _Pnl_tmp]() mutable
        {
            _Pnl_tmp->definirContexto(contextos[glfwWindow], nome_painel, rect);
            paineis.emplace_back(_Pnl_tmp.get()); // Store in the collection
        });
    return _Pnl_tmp;
}

static std::string obterDiretorioDoc()
{
    // Ponteiro para armazenar o caminho
    PWSTR caminhoDocumentos = NULL;
    std::string caminhoDocumentosStr;

    // Obt�m o caminho da pasta Documentos (FOLDERID_Documents)
    HRESULT resultado = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &caminhoDocumentos);

    if (SUCCEEDED(resultado)) {
        // Converte o caminho de wchar_t* para string (opcional)
        _bstr_t bstrPath(caminhoDocumentos);
        caminhoDocumentosStr = bstrPath;
    }
    else {
    }

    // Libera a mem�ria alocada
    if (caminhoDocumentos) {
        CoTaskMemFree(caminhoDocumentos);
    }
    return caminhoDocumentosStr;
}

BubbleUI::Contexto::Contexto(GLFWwindow* window)
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

    // ativa blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ativa o stencil
    glEnable(GL_STENCIL_TEST);

    tamanho = {800, 400};
}

void BubbleUI::novoContexto(GLFWwindow* window)
{
    if (!window) abort();   // Janela inv�lida
    contextos[window] = std::make_shared<Contexto>(window);
    contexto_atual = contextos[window].get();
    rect_vertex.carregado = false;
    linha_vertex.carregado = false;
    halfcircle_vertex.carregado = false;
}

void BubbleUI::adicionarPainel(Contexto* contexto, BubbleUI::Painel* painel)
{
    if (!contexto) return;
    painel->definirContexto(contextos[contexto->glfwWindow]);
    contexto->paineis.emplace_back(painel);
}

void BubbleUI::Contexto::atualizar()
{
    glfwPollEvents();

    // Vetor de futures para armazenar as tarefas ass�ncronas de atualiza��o dos paineis
    std::vector<std::future<void>> futures;

    // Inicia cada painel->atualizar() em uma thread separada
    for (auto& painel : paineis) {
        futures.push_back(std::async(std::launch::async, [&painel]() {
            painel->atualizar();
            }));
    }

    for (auto& future : futures) {
        future.wait();
    }
}
void BubbleUI::Contexto::renderizar() const
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, tamanho.width, tamanho.height);

    for (const auto& painel : paineis)
        painel->renderizar();

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

//static void BubbleUI::renderizarContexto(GLFWwindow* window)
//{
//    if (!window) abort();   // Janela inv�lida
//    if (contextos.find(window) == contextos.end()) abort(); // N�o possui contexto para essa janela
//
//}
void BubbleUI::atualizarContexto(GLFWwindow* window)
{
    Contexto* ctx{ nullptr };
    if (!window) ctx = contexto_atual;   // Janela inv�lida
    else
    if (contextos.find(window) == contextos.end()) ctx = contexto_atual; // N�o possui contexto para essa janela

    if (!ctx) abort(); /// Contexto inv�lido
    ctx->atualizar();
    ctx->renderizar();
}

BubbleUI::Contexto* BubbleUI::janela(const char* title)
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

        // define o �cone da janela
        auto icone_ = Bubble::Arquivadores::ImageLoader("icon.ico");
        const GLFWimage icone = icone_.converterParaGlfw();

        if (icone_.carregado)   glfwSetWindowIcon(janela, 1, &icone);

        // Cria novo contexto ui
        BubbleUI::novoContexto(janela);

        return contextos[janela].get();
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
