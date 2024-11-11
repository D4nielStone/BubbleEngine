
// Copyright (c) 2024 Daniel Oliveira

#pragma once
#include "src/util/includes.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/inputs/inputs.hpp"
#include "bubbleui.hpp"
#include <glfw/glfw3.h>
#include <memory>

/*

O contexto deve ser a base das informações para os painel, além de ser o centro
na renderização e atualização de eventos.

*/
namespace BubbleUI
{
    class Painel; // Declaração antecipada para evitar re-inclusão
    // Contém informações gerais para os paineis e widgets
    struct BEUI_DLL_API Contexto
    {
        GLFWwindow* glfwWindow{ nullptr }; // Janela glfw do contexto em questão
        GLFWcursor* cursor_horizontal{ nullptr };   // Cursor glfw
        GLFWcursor* cursor_texto{ nullptr };        // Cursor glfw
        GLFWcursor* cursor_vertical{ nullptr };     // Cursor glfw
        GLFWcursor* cursor_normal{ nullptr };       // Cursor glfw
        GLFWcursor* cursor{ nullptr }; // Cursor à ser usado
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
    };
    void novoContexto(GLFWwindow* window);
    void adicionarPainel(GLFWwindow* window, Painel* painel);
    void atualizarContexto(GLFWwindow* window);
}