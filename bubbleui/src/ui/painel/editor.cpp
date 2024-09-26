#include "editor.hpp"
#include "src/ui/items/item_botao.hpp"
#include "src/ui/items/item_arvore.hpp"
#include <windows.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*static glm::vec3 worldToScreen(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, int screenWidth, int screenHeight) {
    // Transformação para o espaço da câmera (view space)
    glm::vec4 clipSpacePos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0);

    // Convertemos para NDC (Normalized Device Coordinates)
    if (clipSpacePos.w != 0.0f) {
        clipSpacePos.x /= clipSpacePos.w;
        clipSpacePos.y /= clipSpacePos.w;
        clipSpacePos.z /= clipSpacePos.w;
    }

    // Mapeia as coordenadas NDC para coordenadas de tela
    glm::vec2 screenPos{};
    screenPos.x = (clipSpacePos.x * 0.5f + 0.5f) * screenWidth;
    screenPos.y = (1.0f - (clipSpacePos.y * 0.5f + 0.5f)) * screenHeight; // Inverte o Y para o espaço da tela

    return glm::vec3(screenPos, clipSpacePos.z);
}*/
// Função para selecionar objeto 3D
static void abrirSelecionar()
{
    // Estrutura para armazenar as informações do diálogo
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    wchar_t filtros[120] = L"Modelo FBX\0*.FBX\0Modelo OBJ\0*.OBJ\0Modelo GLTF\0*.GLTF\0Modelo DAE\0*.DAE\0Modelo Blender\0*.BLEND\0";

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
                std::string filePath = std::filesystem::path(ofn.lpstrFile).string();
                Bubble::Cena::criarEntidade(filePath);
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
// Adiciona camera
static void adicionarCamera()
{
    Bubble::Cena::adicionarTarefaNaFila([]()
        {
            Bubble::Cena::criarCamera(Bubble::Cena::CameraEditorAtual()->transformacao->obterPosicao());
        });
}

BubbleUI::Paineis::Editor::Editor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect) : buffer(std::make_shared<BubbleUI::Widgets::Imagem>(0)),
scenemanager(scenemanager)
{
	Nome = "Editor";
    buffer->flip = true;
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
    auto cenaAtual = scenemanager->cenaAtual();
    buffer->defID(cenaAtual->camera_editor.textureColorbuffer);

    if (selecionado) {
        contexto->inputs->setInputMode(InputMode::Editor);
    }
    else if (contexto->inputs->getInputMode() != Game) contexto->inputs->setInputMode(Default);

    if (scenemanager->cenaAtual()->camera_principal)
    {
    }
}