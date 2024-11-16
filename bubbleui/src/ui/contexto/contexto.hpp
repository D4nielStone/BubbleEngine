
// Copyright (c) 2024 Daniel Oliveira

#pragma once
#include "src/util/includes.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/inputs/inputs.hpp"
#include <thread>
#include <future>
#include "bubbleui.hpp"
#include <glfw/glfw3.h>
#include <functional>
#include <queue>
#include <memory>

/*

O contexto deve ser a base das informa��es para os painel, al�m de ser o centro
na renderiza��o e atualiza��o de eventos.

*/
namespace BubbleUI
{
    class Painel; // Declara��o antecipada para evitar re-inclus�o
    // Cont�m informa��es gerais para os paineis e widgets
    class BEUI_DLL_API Contexto
    {
    private:
        std::queue<std::function<void()>> filaDeTarefas;
        std::mutex mutexFilaTarefas;
    public:
        GLFWwindow* glfwWindow{ nullptr }; // Janela glfw do contexto em quest�o
        GLFWcursor* cursor_horizontal{ nullptr };   // Cursor glfw
        GLFWcursor* cursor_texto{ nullptr };        // Cursor glfw
        GLFWcursor* cursor_vertical{ nullptr };     // Cursor glfw
        GLFWcursor* cursor_normal{ nullptr };       // Cursor glfw
        GLFWcursor* cursor{ nullptr }; // Cursor � ser usado
        std::string NomeDoProjeto, VercaoDaEngine{"0.1 Aplha.1"}, VercaoOpengl, NomeGpu, dirDoProjeto; // Flags de controle
        Size tamanho; // Tamanho da janela glfw
        std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr }; // Inputs da janela glfw
        std::vector<Painel*> paineis;
        // Destructor to manage resources
        ~Contexto();
        Contexto(GLFWwindow* window);
        void atualizar();
        void renderizar() const;

        void definirJanela(GLFWwindow* janela);
        void adicionarTarefa(const std::function<void()>& tarefa);

        // adi��o de paineis
        void adicionarVP(const bool preenchido);
    };

    bool parar();
    void novoContexto(GLFWwindow* window);
    std::shared_ptr<Contexto> novoContexto(const char* title, const Vector4& posicao_tamanho = {0, 0, 200, 100});
    void inicializarContextoNaThread(const char* nome_janela, const Vector4& pos_tam);
}