/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "scenemanager.hpp"
#include "src/depuracao/debug.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <src/componentes/prototipo/terreno.hpp>
#include <queue>
#include <filesystem>
#include <rapidjson/writer.h>
#include "src/nucleo/engine.hpp"
#include <mutex>

using namespace Bubble::Cena;

Bubble::Entidades::CameraEditor* camera_editor_atual;
std::shared_ptr<Bubble::Cena::SceneManager> scenemanager;

// O scenemanager gerencia as cenas de um projeto
SceneManager::SceneManager() : currentSceneIndex(-1)
{
}
SceneManager::~SceneManager() {}
// Retorna lista de cenas
std::vector<std::shared_ptr<Scene>> SceneManager::obterCenas() const
{
    return scenes;
}
// Deve retornar Cena atual
std::shared_ptr<Scene> SceneManager::cenaAtual() const {
    if (scenes.size() > 0 && currentSceneIndex > -1)
        return scenes[currentSceneIndex];
    else
    {
        Debug::emitir(Erro, "Não tem cena atual");
        return nullptr;
    }
}
// Deve criar cena padrao, com terreno e esfera
std::shared_ptr<Scene> SceneManager::criarCenaPadrao(std::string Nome)
{
    //Cria cena
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Nome.c_str());

    Cena::camera_editor.transformacao->definirPosicao({ 3, 3, 3 });
    Cena::camera_editor.olharPara({0, 0, 0});
    auto esfera = scene->criarEntidade("assets/primitivas/modelos/sphere.obj");
    auto cubo = scene->criarEntidade("assets/primitivas/modelos/cube.obj");
    if(!esfera->obterFilhos().empty())esfera->obterFilhos()[0]->obterTransformacao()->definirPosicao({0, -1, 0});
    if (!cubo->obterFilhos().empty())cubo->obterFilhos()[0]->obterTransformacao()->definirEscala({2, 0.25f, 2});

    return scene;
}

void SceneManager::salvarCenas()
{
    // Salvar arquivos de cenas individuais
    for (const auto& cena : obterCenas()) {
        rapidjson::Document doc;
        cena->serializar(&doc);

        rapidjson::StringBuffer b;
        rapidjson::Writer<rapidjson::StringBuffer> w(b);
        doc.Accept(w);

        std::ofstream arquivo_cena(Engine::getInstance()->diretorio_projeto + "\\Cenas\\" + cena->nome() + ".bcena");
        if (!arquivo_cena.is_open()) {
            Debug::emitir("Engine", "Erro ao criar o arquivo da cena: " + cena->nome());
            continue;
        }
        arquivo_cena << b.GetString();
        arquivo_cena.close();
    }
}
void SceneManager::carregarCenas(const std::vector<std::string>& cenas)
{
    for (const auto& cena : cenas)
    {
        rapidjson::Document doc;
        const std::string dir = Engine::getInstance()->diretorio_projeto + "\\Cenas\\" + cena + ".bcena";

        // Verifica se o arquivo existe e é válido
        if (!std::filesystem::exists(dir) || !std::filesystem::is_regular_file(dir))
            continue;

        // Lê o conteúdo do arquivo
        std::ifstream arquivo(dir);
        if (!arquivo.is_open())
        {
            Debug::emitir(Erro, "Não foi possível abrir o arquivo: " + dir);
            continue;
        }

        std::stringstream buffer;
        buffer << arquivo.rdbuf();
        arquivo.close();

        // Parseia o conteúdo JSON
        if (doc.Parse(buffer.str().c_str()).HasParseError())
        {
            Debug::emitir(Erro, "Erro ao parsear o arquivo JSON: " + dir);
            continue;
        }

        // Cria a cena e tenta parseá-la
        auto cena_shared = std::make_shared<Scene>(cena);
        if (cena_shared->parse(doc))
            adicionarCena(cena_shared);
        else
            Debug::emitir(Erro, "Parse da cena falhou: " + cena);
    }

    // Cria uma nova cena padrão se nenhuma foi carregada
    if (obterCenas().empty())
        novaCena("Cena", false);
}

// Deve criar e carregar uma nova cena
void SceneManager::novaCena(std::string Nome, bool cenaPadrao)
{
    if (cenaPadrao)
        adicionarCena(criarCenaPadrao(Nome + " " + std::to_string(numeroDeCenas())));
    else
        adicionarCena(std::make_shared<Scene>(Nome + " " + std::to_string(numeroDeCenas())));

    carregarCena(static_cast<int>(numeroDeCenas() - 1));
}

void Bubble::Cena::SceneManager::defJogoViewport(Vector4<int> rect)
{
    viewportJogo = rect;
}

void Bubble::Cena::SceneManager::defEditorViewport(Vector4<int> rect)
{
    viewportEditor = rect;
}
// Deve adicionar cena à lista
void SceneManager::adicionarCena(std::shared_ptr<Scene> scene) 
{
    scenes.push_back(scene);
}
// Deve carregar cena baseado no index
void SceneManager::carregarCena(int sceneIndex) {
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        camera_editor.inputs = inputs;
        currentSceneIndex = sceneIndex;

        std::string msg("Carregando cena ");
        std::string soma = msg + std::to_string(currentSceneIndex);

        Debug::emitir("Cena", soma.c_str());

        scenes[currentSceneIndex]->carregar();
    }
    else {
        Debug::emitir(Erro, "índice da cena não é válido");
    }
}
// Fila de tarefas para o thread principal
std::queue<std::function<void()>> filaDeTarefas;
std::mutex filaMutex;
// Deve renderizar cena atual
void SceneManager::renderizarCenaAtual() const
{
    if (!cenaAtual()) return;

    camera_editor.desenharFrame(viewportEditor);
        cenaAtual()->renderizar(Editor);
    if (cenaAtual()->camera_principal)
    {
        cenaAtual()->camera_principal->desenharFrame(viewportJogo);
        cenaAtual()->renderizar(Game);
    }
}
// Deve atualizar cena atual
void SceneManager::atualizarCenaAtual() const
{

    // Processar fila de tarefas na thread principal
    while (!filaDeTarefas.empty()) {
        filaDeTarefas.front()(); // Executar a tarefa
        filaDeTarefas.pop();      // Remover a tarefa da fila
    }
    float aspectoDoEditor, aspectoDoJogo;
    if (viewportEditor.h != 0)
        aspectoDoEditor = static_cast<float>(viewportEditor.w) / viewportEditor.h;
    else
        aspectoDoEditor = 1;

    if (viewportJogo.h != 0)
        aspectoDoJogo = static_cast<float>(viewportJogo.w) / viewportJogo.h;
    else
        aspectoDoJogo= 1;
    if (cenaAtualIdx() != -1) {
        camera_editor_atual = &camera_editor;
        cenaAtual()->atualizar(aspectoDoEditor, aspectoDoJogo);
    }
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
        camera_editor.inputs = inp;
    
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

void Bubble::Cena::criarEntidade(std::string path)
{
    if (!scenemanager)return;
    if (scenemanager->cenaAtual())scenemanager->cenaAtual()->criarEntidade(path);
}

void Bubble::Cena::criarCamera(const Vector3<float> &posicao)
{
    auto obj = std::make_shared<Entidades::Entidade>("Camera");
    obj->obterTransformacao()->definirPosicao(posicao);
    obj->adicionarComponente(std::make_shared<Componentes::Camera>());
    scenemanager->cenaAtual()->adicionarEntidade(obj);
}
void Bubble::Cena::adicionarCena()
{
    scenemanager->novaCena();
}
