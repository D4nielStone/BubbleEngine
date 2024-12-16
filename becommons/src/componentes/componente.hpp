/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file componente.hpp
 * @brief Estrutura base para componetes
 */
#pragma once
#include <cstdint>

namespace bubble
{
	/**
	* @enum mascara
	* @brief facilita a identificacao dos componentes pertencentes a uma entidade
	*/
	enum mascara : uint32_t
	{
		COMPONENTE_NONE				= 0,
		COMPONENTE_CAM				= 1 << 0,
		COMPONENTE_RENDER			= 1 << 1,
		COMPONENTE_PROPRIEDADES		= 1 << 2,
		COMPONENTE_TRANSFORMACAO	= 1 << 3,
	};
	// Operadores bit a bit para mascara
	inline mascara operator|(mascara lhs, mascara rhs) {
		using T = std::underlying_type_t<mascara>;
		return static_cast<mascara>(static_cast<T>(lhs) | static_cast<T>(rhs));
	}
	inline mascara& operator|=(mascara& lhs, mascara rhs) {
		lhs = lhs | rhs;
		return lhs;
	}
	inline mascara operator&(mascara lhs, mascara rhs) {
		using T = std::underlying_type_t<mascara>;
		return static_cast<mascara>(static_cast<T>(lhs) & static_cast<T>(rhs));
	}
	inline mascara& operator&=(mascara& lhs, mascara rhs) {
		lhs = lhs & rhs;
		return lhs;
	}
	inline mascara operator~(mascara mask) {
		using T = std::underlying_type_t<mascara>;
		return static_cast<mascara>(~static_cast<T>(mask));
	}

	/**
	* @struct componente
	* @brief base para as funcionalidades da engine
	*/
	struct componente
	{
		virtual ~componente() = default;
	};
}