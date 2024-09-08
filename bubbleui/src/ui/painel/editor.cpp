#include "editor.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/ui/items/item_arvore.hpp"
#include <windows.h>
#include <filesystem>

// Função para selecionar objeto 3D
static void abrirSelecionar()
{
    // Estrutura para armazenar as informações do diálogo
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    wchar_t filtros[120] = L"Todos os Arquivos\0*.*\0Modelo FBX\0*.FBX\0Modelo OBJ\0*.OBJ\0Modelo GLTF\0*.GLTF\0Modelo DAE\0*.DAE\0Modelo Blender\0*.BLEND\0";

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
        Bubble::Cena::adicionarTarefaNaFila([ofn]()
            {
                Bubble::Cena::criarEntidade(std::filesystem::path(ofn.lpstrFile).string());
            });
    }
}
// Função para adicionar Cubo
static void adicionarCubo()
{
    Bubble::Cena::adicionarTarefaNaFila([]()
        {
            Bubble::Cena::criarEntidade("assets/primitivas/modelos/cube.dae");
        });
}
// Função para adicionar Esfera
static void adicionarEsfera()
{
    Bubble::Cena::adicionarTarefaNaFila([]()
        {
            Bubble::Cena::criarEntidade("assets/primitivas/modelos/sphere.dae");
        });
}// Função para adicionar Camera
static void adicionarCamera()
{
    Bubble::Cena::adicionarTarefaNaFila([]()
        {
            Bubble::Cena::criarCamera(Bubble::Cena::CameraEditorAtual()->transformacao->obterPosicao());
        });
}

BubbleUI::Paineis::Editor::Editor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect) : buffer(std::make_shared<BubbleUI::Widgets::Imagem>(0))
, scenemanager(scenemanager)
{
	Nome = "Editor";
	renderizarCorpo = false;
	configurar(ctx, rect);
	adicionarWidget(buffer);

    // Popup para adicionar primitivas
    auto popup_primitivas = std::make_shared<Util::PopUp>(ctx);
    popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Cubo", &adicionarCubo));
    popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Esfera", &adicionarEsfera));
    popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Camera", &adicionarCamera));
    //popup_primitivas->adiItem(std::make_shared<Items::Botao>("adicionar Plano", &abrirSelecionar));
    // Popup principal
	menuDeContexto->adiItem(std::make_shared<Items::Botao>("importar objeto 3D", &abrirSelecionar));
	menuDeContexto->adiItem(std::make_shared<Items::Arvore>("adicionar primitiva", popup_primitivas));
}

void BubbleUI::Paineis::Editor::preAtualizacao()
{
    Vector4 rect_size = buffer->obtRect();
    scenemanager->defEditorViewport(rect_size);
    buffer->defID(scenemanager->cenaAtual()->camera_editor.textureColorbuffer);

    if (selecionado) {
        contexto->inputs->setInputMode(InputMode::Editor);
    }
    else             contexto->inputs->setInputMode(InputMode::Default);
}
