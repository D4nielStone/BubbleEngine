#include "scenemanager.hpp"
#include "src/depuracao/debug.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <src/componentes/prototipo/terreno.hpp>

using namespace Bubble::Cena;

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
        Debug::emitir(Debug::Erro, "N�o tem cena atual");
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

void Bubble::Cena::SceneManager::defViewport(Vector4 rect)
{
    viewport_rect = rect;
}
// Deve adicionar cena � lista
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
        Debug::emitir(Debug::Tipo::Erro, "�ndice da cena n�o � v�lido");
    }
}
// Deve renderizar cena atual
void SceneManager::renderizarCenaAtual() const
{

    float aspecto;
    if (viewport_rect.h != 0)
        aspecto = static_cast<float>(viewport_rect.w) / viewport_rect.h;
    else
        aspecto = 1;
    glViewport(0, 0, viewport_rect.w, viewport_rect.h);
    glEnable(GL_DEPTH_TEST);

    cenaAtual()->camera_editor.desenharFrame(viewport_rect);
    cenaAtual()->renderizar(aspecto);
    // Desligar framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// Deve atualizar cena atual
void SceneManager::atualizarCenaAtual(float deltaTime) const
{
    cenaAtual()->atualizar(deltaTime);
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
void SceneManager::defIputs(Inputs::Inputs* inp)
{
    inputs = inp;
    for (auto& scene : scenes)
    {
        scene->camera_editor.inputs = inp;
    }
}

