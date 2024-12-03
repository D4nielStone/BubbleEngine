/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file contexto.hpp
 * @brief Declara a estrutura `Contexto` e fun��es relacionadas.
 *
 * A estrutura `Contexto` serve como base para armazenar informa��es gerais sobre a aplica��o,
 * incluindo a janela GLFW, inputs e pain�is associados. Tamb�m gerencia a renderiza��o e a atualiza��o de eventos
 * no framework BubbleUI.
 */

#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "bubbleui.hpp"
#include "src/util/includes.hpp"
#include "src/inputs/inputs.hpp"


namespace BubbleUI
{
    class Painel; ///< Declara��o antecipada para evitar depend�ncia c�clica.

    /**
     * @struct Contexto
     * @brief Estrutura que representa o estado compartilhado e gerencia informa��es de renderiza��o e eventos.
     *
     * A estrutura `Contexto` centraliza as opera��es do framework, gerenciando:
     * - Estado e recursos associados � janela GLFW.
     * - Renderiza��o de pain�is e widgets.
     * - Gerenciamento de eventos, como entrada de usu�rio.
     */
    struct BEUI_DLL_API Contexto
    {
        /// Ponteiro para a janela GLFW associada ao contexto.
        GLFWwindow* glfwWindow{ nullptr };

        /// Cursores personalizados para intera��es espec�ficas.
        GLFWcursor* cursor_horizontal{ nullptr }; ///< Cursor horizontal.
        GLFWcursor* cursor_texto{ nullptr };      ///< Cursor de texto.
        GLFWcursor* cursor_vertical{ nullptr };   ///< Cursor vertical.
        GLFWcursor* cursor_normal{ nullptr };     ///< Cursor padr�o.
        GLFWcursor* cursor{ nullptr }; ///< Cursor atualmente em uso.

        /// Informa��es de identifica��o do projeto.
        std::string NomeDoProjeto;               ///< Nome do projeto.
        std::string VercaoDaEngine{ "0.1 Alpha.1" }; ///< Vers�o da engine.
        std::string VercaoOpengl;                ///< Vers�o do OpenGL usada.
        std::string NomeGpu;                     ///< Nome da GPU em uso.
        std::string dirDoProjeto;                ///< Diret�rio raiz do projeto.

        /// Dimens�es da janela GLFW.
        Size tamanho;
        /// Resolucao da fonte.
        size_t resolucao{ 15 };

        /// Gerenciamento de entradas do usu�rio.
        std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };

        /// Lista de ponteiros para pain�is associados ao contexto.
        std::vector<Painel*> paineis;

        /**
         * @brief Destrutor.
         *
         * Libera os recursos associados, como cursores GLFW.
         */
        ~Contexto();

        /**
         * @brief Construtor.
         *
         * Inicializa um contexto com uma janela GLFW.
         *
         * @param window Ponteiro para a janela GLFW.
         */
        Contexto(GLFWwindow* window);

        /**
         * @brief Atualiza o estado do contexto.
         *
         * Respons�vel por processar eventos e atualizar os estados dos pain�is e widgets.
         */
        void atualizar();

        /**
         * @brief Renderiza o contexto.
         *
         * Chama as opera��es de renderiza��o associadas aos pain�is e widgets registrados.
         */
        void renderizar();

        /**
         * @brief Cria ou obt�m um painel associado ao contexto.
         *
         * @param nome_painel Nome identificador do painel.
         * @param rect Ret�ngulo que define as dimens�es do painel.
         * @return Um ponteiro compartilhado para o painel criado ou existente.
         */
        std::shared_ptr<Painel> painel(const char* nome_painel, const Vector4& rect);

        /**
         * @brief Define a janela GLFW associada ao contexto.
         *
         * @param janela Ponteiro para a nova janela GLFW.
         */
        void definirJanela(GLFWwindow* janela);
    };

    /**
     * @brief Cria um novo contexto.
     *
     * Inicializa um contexto e associa-o a uma janela GLFW.
     *
     * @param window Ponteiro para a janela GLFW.
     */
    void novoContexto(GLFWwindow* window);

    /**
     * @brief Adiciona um painel ao contexto atual.
     *
     * @param contexto Ponteiro para o contexto em questao.
     * @param painel Ponteiro para o painel a ser adicionado.
     */
    void adicionarPainel(Contexto* contexto, Painel* painel);

    /**
     * @brief Atualiza o contexto associado a uma janela GLFW.
     */
    void atualizarContexto();
    /**
     * @brief Renderiza o contexto associado a uma janela GLFW.
     */
    void renderizarContexto();

    /**
    * @brief Cria uma nova janela GLFW
    * @brief A janela sera a principal
    * 
    * @param title Titulo da janela
    * @return Retorna o ponteiro do contexto
    */
    std::shared_ptr<BubbleUI::Contexto> janela(const char* title);

    /**
    * @brief Flag para atualizar contexto
    * @return Boleano para atualizacao
    */

    bool fim();

    /**
    * @brief Adiciona uma tarefa para ser executada na thread peincipal apos a renderizacao.
    * 
    * @param funcao Funcao a ser executada
    */
    void tarefa(const std::function<void()> funcao);
}
