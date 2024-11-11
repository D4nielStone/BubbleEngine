// Copyright (c) 2024 Daniel Oliveira

#include <glad/glad.h>
#include "contexto.hpp"
#include <shlobj.h> // Necessário para SHGetKnownFolderPath
#include <map>
#include <comdef.h> // Necessário para _bstr_t (conversão de wchar_t para string)
#include "src/ui/painel/painel.hpp"

// contextos existentes
std::map<GLFWwindow*, std::shared_ptr<BubbleUI::Contexto>> contextos;

// Contexto padrão
BubbleUI::Contexto* contexto_atual{ nullptr };

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

BubbleUI::Contexto::Contexto(GLFWwindow* window)
{

    cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
    cursor_texto = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursor = cursor_normal;
    dirDoProjeto = obterDiretorioDoc() + "/Bubble Engine/Projetos";
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
    contexto_atual = this;

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
    if (!window) abort();   // Janela inválida
    contextos[window] = std::make_shared<Contexto>(window);
}

void BubbleUI::adicionarPainel(GLFWwindow* window, BubbleUI::Painel* painel)
{
    if (contextos.find(window) == contextos.end()) abort(); // Não possui contexto para essa janela
    painel->definirContexto(contextos[window]);
    contextos[window]->paineis.emplace_back(painel);
}

void BubbleUI::Contexto::atualizar()
{
    glfwPollEvents();
    for (auto& painel : paineis)
        painel->atualizar();
}
void BubbleUI::Contexto::renderizar() const
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, tamanho.width, tamanho.height);

    for (const auto& painel : paineis)
        painel->renderizar();

    glfwSwapBuffers(glfwWindow);
}

//static void BubbleUI::renderizarContexto(GLFWwindow* window)
//{
//    if (!window) abort();   // Janela inválida
//    if (contextos.find(window) == contextos.end()) abort(); // Não possui contexto para essa janela
//
//}
void BubbleUI::atualizarContexto(GLFWwindow* window)
{
    if (!window) abort();   // Janela inválida
    if (contextos.find(window) == contextos.end()) abort(); // Não possui contexto para essa janela

    contextos[window]->atualizar();
    contextos[window]->renderizar();
}