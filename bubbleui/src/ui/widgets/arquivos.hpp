// Copyright (c) 2024 Daniel Oliveira

/**
 * @file arquivos.hpp
 * @brief Declara a classe `Arquivos`, que exibe e gerencia arquivos e pastas em um diretório.
 */

#pragma once
#include "widget.hpp"
#include "caixa_de_texto.hpp"
#include "botao.hpp"
#include "texto.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		/**
		 * @class Arquivos
		 * @brief Widget para exibir e interagir com arquivos e pastas em um diretório.
		 *
		 * Esta classe permite renderizar uma interface gráfica para exibir e manipular
		 * arquivos e pastas contidos em um diretório específico.
		 * Contém elementos interativos, como botões e uma caixa de texto.
		 */
		class BEUI_DLL_API Arquivos : public Widget
		{
		public:
			/**
			 * @brief Construtor padrão da classe `Arquivos`.
			 */
			Arquivos();

			/**
			 * @brief Atualiza o estado do widget.
			 *
			 * Este método é chamado regularmente para atualizar o estado interno do widget
			 * com base em eventos ou mudanças no contexto do aplicativo.
			 */
			void atualizar() override;

			/**
			 * @brief Renderiza o widget na interface gráfica.
			 *
			 * Este método é responsável por desenhar o widget e seus componentes
			 * na tela.
			 */
			void renderizar() const override;

			/**
			 * @brief Recarrega os arquivos e pastas de um diretório.
			 *
			 * @param dir Caminho para o diretório a ser carregado.
			 * @param tipo_arquivo Tipo de arquivo aceito, originalmente '*' para todos os arquivos.
			 *
			 * Este método limpa o estado atual e carrega os arquivos e pastas
			 * do diretório especificado.
			 */
			void recarregar(const char* dir, const char* tipo_arquivo = "*");

		private:
			/**
			 * @brief Adiciona um botão ao widget.
			 *
			 * @param label Texto exibido no botão.
			 * @param icone Caminho ou identificador do ícone associado ao botão.
			 */
			void adiBotao(const std::string& label, const std::string& icone);

			/// Vetor de ponteiros exclusivos para botões exibidos no widget.
			std::vector<std::unique_ptr<Botao>> botoes;

			/// Lista de pares representando os diretórios (nome e caminho).
			std::vector<std::pair<std::string, std::string>> diretorios;

			/// Caixa de texto usada para exibir ou editar o caminho do diretório.
			std::unique_ptr<CaixaTexto> text_box{ nullptr };
		};
	}
}