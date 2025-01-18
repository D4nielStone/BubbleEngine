/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "transformacao.hpp"
#include <string>
#include "src/depuracao/debug.hpp"
#include <lua.hpp>

namespace bubble
{
	struct codigo : componente
	{
		static constexpr mascara mascara = COMPONENTE_CODIGO;
		const std::string arquivo;
		lua_State* L;

		/**
		* @brief carrega script lua
		*/
		codigo(const std::string& arquivo);
		/**
		* @brief limpa recursos
		*/
		~codigo();
		void iniciar() const;
		void encerrar();
		void atualizar() const;
	};
}