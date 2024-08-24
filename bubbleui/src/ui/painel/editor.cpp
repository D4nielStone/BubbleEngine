#include "editor.hpp"
#include "src/ui/items/item_botao.hpp"
#include "windows.h"

std::wstring path = L"";
static void abrirSelecionar()
{
    // Estrutura para armazenar as informações do diálogo
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    wchar_t filtros[47] = L"Todos os Arquivos\0*.*\0Arquivos de Texto\0*.TXT\0";

    // Inicializa a estrutura com zeros
    ZeroMemory(&ofn, sizeof(ofn));

    // Configura as opções do diálogo
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // Pode ser o handle da janela principal da sua UI
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filtros;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Exibe o diálogo de seleção de arquivo
    if (GetOpenFileName(&ofn) == TRUE)
    {
        path = ofn.lpstrFile;
    }
}
BubbleUI::Paineis::Editor::Editor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager), buffer(new Widgets::Imagem(0))
{
	Nome = "Editor";
	renderizar_corpo = false;
	configurar(ctx, rect);
	adiWidget(buffer);
	menu_de_contexto->adiItem(new Items::Botao("adicionar objeto 3D", &abrirSelecionar));
}

void BubbleUI::Paineis::Editor::preAtualizacao()
{
	if (scenemanager->cenaAtualIdx() != -1)
	{
        if (path != L"")
        {
            scenemanager->cenaAtual()->adicionarEntidade(std::make_unique<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d(path)));
            path = L"";
        }
		Vector4 rect_size = buffer->obtRect();
		scenemanager->defViewport(rect_size);
		buffer->defID(scenemanager->cenaAtual()->camera_editor.textureColorbuffer);
	}
}
