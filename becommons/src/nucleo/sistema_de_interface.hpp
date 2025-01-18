/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_interface.hpp
 * @brief Gerencia a interface de jogo
 */

#pragma once
#include "sistema.hpp"
#include "src/arquivadores/shader.hpp"
#include "src/componentes/texto.hpp"
#include "src/componentes/imagem.hpp"

namespace bubble
{
	class sistemaInterface : public sistema
	{
	public:
		sistemaInterface() = default;
		~sistemaInterface() override;
		void atualizar() override;
		void inicializar(bubble::fase* fase) override;
		static void desenharTexto(shader& s, const texto& t);
		static void desenharImagem(shader& s, const imagem& img);
	private:
		inline static unsigned int text_VAO, text_VBO;
		inline static unsigned int img_VAO, img_VBO, img_EBO;
	};
}