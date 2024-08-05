#include "scenemanager.h"
#include "becore.h"
#include "src/depuracao/debug.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <src/componentes/prototipo/terreno.h>

namespace Bubble::Nucleo
{
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
            Debug::emitir(Debug::Erro, "Não tem cena atual");
    }
    // Deve criar cena padrao, com terreno e esfera
    Scene* SceneManager::criarCenaPadrao(std::string Nome)
    {
        //Cria cena
        Scene* scene = new Scene(Nome.c_str());
        //Cira e configura entidade Terreno
        auto terreno = std::make_shared<Bubble::Entidades::Entidade>("Terreno");
        terreno->adicionarComponente(std::make_shared<Bubble::Componentes::Terreno>());

        //Cria e configura entidade Esfera
        auto esfera = std::make_shared<Bubble::Entidades::Entidade>(Bubble::Arquivadores::Arquivo3d("assets/primitivas/modelos/sphere.dae"));

        // Adiciona Entidades
        scene->adicionarEntidade(esfera);
        scene->adicionarEntidade(terreno);

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
    // Deve renderizar cena atual
    void SceneManager::renderizarCenaAtual(Vector2 viewportSize) const
    {
        float aspecto;
        if (viewportSize.h != 0)
            aspecto = static_cast<float>(viewportSize.w) / viewportSize.h;
        else
            aspecto = 1;
        glViewport(0, 0, viewportSize.w, viewportSize.h);
        glEnable(GL_DEPTH_TEST);

        cenaAtual()->renderizar(aspecto);
    }
    // Deve atualizar cena atual
    void SceneManager::atualizarCenaAtual(float deltaTime) const
    {
        cenaAtual()->atualizar(deltaTime);
    }
    // Deve retornar numero de cenas
    int SceneManager::numeroDeCenas() const {
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
}
