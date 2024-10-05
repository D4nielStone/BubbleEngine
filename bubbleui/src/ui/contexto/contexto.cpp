#include "contexto.hpp"
#include <shlobj.h> // Necessário para SHGetKnownFolderPath
#include <comdef.h> // Necessário para _bstr_t (conversão de wchar_t para string)


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
    dirDoProjeto = obterDiretorioDoc() + "/ProjetosBubble";
}

BubbleUI::Contexto::~Contexto() = default;