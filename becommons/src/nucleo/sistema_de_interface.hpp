/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_interface.hpp
 * @brief Gerencia a interface de jogo
 */

#pragma once
#include "sistema.hpp"
#include "src/arquivadores/shader.hpp"

namespace bubble
{
	class sistemaInterface : public sistema
	{
	public:
		sistemaInterface() = default;
		void atualizar(double deltaTime) override;
		void inicializar(bubble::cena* cena) override;
		void texto(shader& s, const std::string& texto, float x, float y, float escala, bubble::cor color);
	private:
		unsigned int text_VAO, text_VBO;
	};
}