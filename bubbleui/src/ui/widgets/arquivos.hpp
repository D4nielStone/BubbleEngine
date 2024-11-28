// Copyright (c) 2024 Daniel Oliveira

/**
 * @file arquivos.hpp
 * @brief Declara a classe `Arquivos`, que exibe e gerencia arquivos e pastas em um diret�rio.
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
		 * @brief Widget para exibir e interagir com arquivos e pastas em um diret�rio.
		 *
		 * Esta classe permite renderizar uma interface gr�fica para exibir e manipular
		 * arquivos e pastas contidos em um diret�rio espec�fico.
		 * Cont�m elementos interativos, como bot�es e uma caixa de texto.
		 */
		class BEUI_DLL_API Arquivos : public Widget
		{
		public:
			/**
			 * @brief Construtor padr�o da classe `Arquivos`.
			 */
			Arquivos();

			/**
			 * @brief Atualiza o estado do widget.
			 *
			 * Este m�todo � chamado regularmente para atualizar o estado interno do widget
			 * com base em eventos ou mudan�as no contexto do aplicativo.
			 */
			void atualizar() override;

			/**
			 * @brief Renderiza o widget na interface gr�fica.
			 *
			 * Este m�todo � respons�vel por desenhar o widget e seus componentes
			 * na tela.
			 */
			void renderizar() const override;

			/**
			 * @brief Recarrega os arquivos e pastas de um diret�rio.
			 *
			 * @param dir Caminho para o diret�rio a ser carregado.
			 * @param tipo_arquivo Tipo de arquivo aceito, originalmente '*' para todos os arquivos.
			 *
			 * Este m�todo limpa o estado atual e carrega os arquivos e pastas
			 * do diret�rio especificado.
			 */
			void recarregar(const char* dir, const char* tipo_arquivo = "*");

		private:
			/**
			 * @brief Adiciona um bot�o ao widget.
			 *
			 * @param label Texto exibido no bot�o.
			 * @param icone Caminho ou identificador do �cone associado ao bot�o.
			 */
			void adiBotao(const std::string& label, const std::string& icone);

			/// Vetor de ponteiros exclusivos para bot�es exibidos no widget.
			std::vector<std::unique_ptr<Botao>> botoes;

			/// Lista de pares representando os diret�rios (nome e caminho).
			std::vector<std::pair<std::string, std::string>> diretorios;

			/// Caixa de texto usada para exibir ou editar o caminho do diret�rio.
			std::unique_ptr<CaixaTexto> text_box{ nullptr };
		};
	}
}