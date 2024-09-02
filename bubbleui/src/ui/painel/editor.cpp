#include "editor.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/ui/items/item_arvore.hpp"
#include <windows.h>
#include <filesystem>

std::wstring path = L"";
// Fun��o para selecionar objeto 3D
static void abrirSelecionar()
{
    // Estrutura para armazenar as informa��es do di�logo
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    wchar_t filtros[120] = L"Todos os Arquivos\0*.*\0Modelo FBX\0*.FBX\0Modelo OBJ\0*.OBJ\0Modelo GLTF\0*.GLTF\0Modelo DAE\0*.DAE\0Modelo Blender\0*.BLEND\0";

    // Inicializa a estrutura com zeros
    ZeroMemory(&ofn, sizeof(ofn));

    // Configura as op��es do di�logo
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

    // Exibe o di�logo de sele��o de arquivo
    if (GetOpenFileName(&ofn) == TRUE)
    {
        path = ofn.lpstrFile;
    }
}
// Fun��o para adicionar Cubo
static void adicionarCubo()
{
    path = L"assets/primitivas/modelos/cube.dae";
}
// Fun��o para adicionar Esfera
static void adicionarEsfera()
{
    path = L"assets/primitivas/modelos/sphere.dae";
}

BubbleUI::Paineis::Editor::Editor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect) : scenemanager(scenemanager), buffer(std::make_shared<Widgets::Imagem>(0))
{
	Nome = "Editor";
	renderizar_corpo = false;
	configurar(ctx, rect);
	adiWidget(buffer);

    // Popup para adicionar primitivas
    auto popup_primitivas = std::make_shared<Util::PopUp>(ctx);
    popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Cubo", &adicionarCubo));
    popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Esfera", &adicionarEsfera));
    //popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Cilindro", &abrirSelecionar));
    //popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Plano", &abrirSelecionar));
    // Popup principal
	menu_de_contexto->adiItem(std::make_shared<Items::Botao>("importar objeto 3D", &abrirSelecionar));
	menu_de_contexto->adiItem(std::make_shared<Items::Arvore>("adicionar primitiva", popup_primitivas));
}

void BubbleUI::Paineis::Editor::preAtualizacao()
{
    // Adicionar objeto caso "path" esteja preenchido
    if (!selecionado)return;
    if (!path.empty())
    {
        std::string novo_path = std::filesystem::path(path).string();
        Bubble::Cena::adicionarTarefaNaFila([this, novo_path]()
            {
                Bubble::Cena::criarEntidade(scenemanager, novo_path);
            });
        path = L"";
    }
    buffer->defID(scenemanager->cenaAtual()->camera_editor.textureColorbuffer);
    Vector4 rect_size = buffer->obtRect();
    scenemanager->defViewport(rect_size);

    if (selecionado) contexto->inputs->setInputMode(InputMode::Editor);
    else             contexto->inputs->setInputMode(InputMode::Default);
}
