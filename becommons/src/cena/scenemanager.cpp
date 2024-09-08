#include "scenemanager.hpp"
#include "src/depuracao/debug.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <src/componentes/prototipo/terreno.hpp>
#include <queue>
#include <mutex>

using namespace Bubble::Cena;

Bubble::Entidades::CameraEditor* camera_editor_atual;
std::shared_ptr<Bubble::Cena::SceneManager> scenemanager;

// O scenemanager gerencia as cenas de um projeto
SceneManager::SceneManager() : currentSceneIndex(-1)
{
}
SceneManager::~SceneManager() {}
// Deve retornar Cena atual
Scene* SceneManager::cenaAtual() const {
    if (scenes.size() > 0 && currentSceneIndex > -1)
        return scenes[currentSceneIndex];
    else
    {
        Debug::emitir(Debug::Erro, "Não tem cena atual");
        return nullptr;
    }
}
// Deve criar cena padrao, com terreno e esfera
Scene* SceneManager::criarCenaPadrao(std::string Nome)
{
    //Cria cena
    Scene* scene = new Scene(Nome.c_str());
    
    scene->camera_editor.transformacao->definirPosicao({3, 3, 3});
    scene->camera_editor.transformacao->Rotacionar(0.f, -45.f, -45.f);

    return scene;
}
// Deve criar e carregar uma nova cena
void SceneManager::novaCena(std::string Nome, bool cenaPadrao)
{
    if (cenaPadrao)
        adicionarCena(criarCenaPadrao(Nome));
    else
        adicionarCena(new Scene(Nome.c_str()));
    carregarCena(numeroDeCenas() - 1);
}

void Bubble::Cena::SceneManager::defJogoViewport(Vector4 rect)
{
    viewportJogo = rect;
}

void Bubble::Cena::SceneManager::defEditorViewport(Vector4 rect)
{
    viewportEditor = rect;
}
// Deve adicionar cena à lista
void SceneManager::adicionarCena(Scene* scene) 
{
    scenes.push_back(scene);
}
// Deve carregar cena baseado no index
void SceneManager::carregarCena(int sceneIndex) {
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        scenes[sceneIndex]->camera_editor.inputs = inputs;
        currentSceneIndex = sceneIndex;

        std::string msg("Carregando cena ");
        std::string soma = msg + std::to_string(currentSceneIndex);

        Debug::emitir(Debug::Tipo::Mensagem, soma.c_str());

        scenes[currentSceneIndex]->carregar();
    }
    else {
        Debug::emitir(Debug::Tipo::Erro, "índice da cena não é válido");
    }
}
// Fila de tarefas para o thread principal
std::queue<std::function<void()>> filaDeTarefas;
std::mutex filaMutex;
// Deve renderizar cena atual
void SceneManager::renderizarCenaAtual() const
{
    // Processar fila de tarefas na thread principal
    while (!filaDeTarefas.empty()) {
        filaDeTarefas.front()(); // Executar a tarefa
        filaDeTarefas.pop();      // Remover a tarefa da fila
    }
    glViewport(0, 0, viewportEditor.w, viewportEditor.h);
    glEnable(GL_DEPTH_TEST);

    cenaAtual()->camera_editor.desenharFrame(viewportEditor);
    cenaAtual()->renderizar(Editor);
    if (cenaAtual()->camera_principal)
    {
    glViewport(0, 0, viewportJogo.w, viewportJogo.h);
        cenaAtual()->camera_principal->desenharFrame(viewportJogo);
        cenaAtual()->renderizar(Game);
    }
    // Desligar framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// Deve atualizar cena atual
void SceneManager::atualizarCenaAtual() const
{
    float aspectoDoEditor, aspectoDoJogo;
    if (viewportEditor.h != 0)
        aspectoDoEditor = static_cast<float>(viewportEditor.w) / viewportEditor.h;
    else
        aspectoDoEditor = 1;

    if (viewportJogo.h != 0)
        aspectoDoJogo = static_cast<float>(viewportJogo.w) / viewportJogo.h;
    else
        aspectoDoJogo= 1;

    camera_editor_atual = &cenaAtual()->camera_editor;
    cenaAtual()->atualizar(aspectoDoEditor, aspectoDoJogo);
}
// Deve retornar numero de cenas
size_t SceneManager::numeroDeCenas() const {
    return scenes.size();
}
// Deve retornar index da cena atual
int SceneManager::cenaAtualIdx() const {
    return currentSceneIndex;
}
// Deve definir inputs
void SceneManager::defIputs(std::shared_ptr<Inputs::Inputs> inp)
{
    inputs = inp;
    for (auto& scene : scenes)
    {
        scene->camera_editor.inputs = inp;
    }
}

Bubble::Entidades::CameraEditor* Bubble::Cena::CameraEditorAtual()
{
    return camera_editor_atual;
}

std::shared_ptr<Bubble::Cena::SceneManager> Bubble::Cena::obterSceneManager()
{
    return scenemanager;
}

void Bubble::Cena::definirSceneManager(std::shared_ptr<SceneManager> scene_manager)
{
    scenemanager = scene_manager;
}

// Função para adicionar uma tarefa na fila
static void Bubble::Cena::adicionarTarefaNaFila(std::function<void()> tarefa)
{
    std::lock_guard<std::mutex> lock(filaMutex);
    filaDeTarefas.push(tarefa);
}

void Bubble::Cena::criarEntidade(std::string path)
{
    scenemanager->cenaAtual()->criarEntidade(std::make_unique<Arquivadores::Arquivo3d>(path));
}

void Bubble::Cena::criarCamera(glm::vec3 posicao)
{
    auto obj = std::make_shared<Entidades::Entidade>("Camera");
    obj->obterTransformacao()->definirPosicao(posicao);
    obj->adicionarComponente(std::make_shared<Componentes::Camera>());
    scenemanager->cenaAtual()->adicionarEntidade(obj);
}
