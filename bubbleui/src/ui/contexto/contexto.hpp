/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file contexto.hpp
 * @brief Declara a estrutura `Contexto` e funções relacionadas.
 *
 * A estrutura `Contexto` serve como base para armazenar informações gerais sobre a aplicação,
 * incluindo a janela GLFW, inputs e painéis associados. Também gerencia a renderização e a atualização de eventos
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
    class Painel; ///< Declaração antecipada para evitar dependência cíclica.

    /**
     * @struct Contexto
     * @brief Estrutura que representa o estado compartilhado e gerencia informações de renderização e eventos.
     *
     * A estrutura `Contexto` centraliza as operações do framework, gerenciando:
     * - Estado e recursos associados à janela GLFW.
     * - Renderização de painéis e widgets.
     * - Gerenciamento de eventos, como entrada de usuário.
     */
    struct BEUI_DLL_API Contexto
    {
        /// Ponteiro para a janela GLFW associada ao contexto.
        GLFWwindow* glfwWindow{ nullptr };

        /// Cursores personalizados para interações específicas.
        GLFWcursor* cursor_horizontal{ nullptr }; ///< Cursor horizontal.
        GLFWcursor* cursor_texto{ nullptr };      ///< Cursor de texto.
        GLFWcursor* cursor_vertical{ nullptr };   ///< Cursor vertical.
        GLFWcursor* cursor_normal{ nullptr };     ///< Cursor padrão.
        GLFWcursor* cursor{ nullptr }; ///< Cursor atualmente em uso.

        /// Informações de identificação do projeto.
        std::string NomeDoProjeto;               ///< Nome do projeto.
        std::string VercaoDaEngine{ "0.1 Alpha.1" }; ///< Versão da engine.
        std::string VercaoOpengl;                ///< Versão do OpenGL usada.
        std::string NomeGpu;                     ///< Nome da GPU em uso.
        std::string dirDoProjeto;                ///< Diretório raiz do projeto.

        /// Dimensões da janela GLFW.
        Size tamanho;
        /// Resolucao da fonte.
        size_t resolucao{ 15 };

        /// Gerenciamento de entradas do usuário.
        std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };

        /// Lista de ponteiros para painéis associados ao contexto.
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
         * Responsável por processar eventos e atualizar os estados dos painéis e widgets.
         */
        void atualizar();

        /**
         * @brief Renderiza o contexto.
         *
         * Chama as operações de renderização associadas aos painéis e widgets registrados.
         */
        void renderizar();

        /**
         * @brief Cria ou obtém um painel associado ao contexto.
         *
         * @param nome_painel Nome identificador do painel.
         * @param rect Retângulo que define as dimensões do painel.
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
