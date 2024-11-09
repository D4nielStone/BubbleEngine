// Copyright (c) 2024 Daniel Oliveira

#include "contexto.hpp"
#include <shlobj.h> // Necessário para SHGetKnownFolderPath
#include <comdef.h> // Necessário para _bstr_t (conversão de wchar_t para string)


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

BubbleUI::Contexto::Contexto()
{

    cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
    cursor_texto = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursor = cursor_normal;
    dirDoProjeto = obterDiretorioDoc() + "/Bubble Engine/Projetos";
}

BubbleUI::Contexto::~Contexto() = default;

static void callback_size(GLFWwindow* janela, int width, int height)
{
    contexto_atual->tamanho.height = height;
    contexto_atual->tamanho.width = width;
}

void BubbleUI::Contexto::def_inputs(GLFWwindow* janela)
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
    tamanho = {800, 400};
}
