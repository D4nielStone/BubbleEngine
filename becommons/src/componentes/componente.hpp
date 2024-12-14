/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file componente.hpp
 * @brief Estrutura base para componetes
 */
#pragma once
#include <cstdint>

enum MascaraComponente : uint32_t
{
	COMPONENTE_NONE          = 0,
	COMPONENTE_CAM           = 1 << 0,
	COMPONENTE_RENDER		 = 1 << 1,
	COMPONENTE_PROPRIEDADES  = 1 << 2,
};
// Operadores bit a bit para MascaraComponente
inline MascaraComponente operator|(MascaraComponente lhs, MascaraComponente rhs) {
	using T = std::underlying_type_t<MascaraComponente>;
	return static_cast<MascaraComponente>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline MascaraComponente& operator|=(MascaraComponente& lhs, MascaraComponente rhs) {
	lhs = lhs | rhs;
	return lhs;
}
inline MascaraComponente operator&(MascaraComponente lhs, MascaraComponente rhs) {
	using T = std::underlying_type_t<MascaraComponente>;
	return static_cast<MascaraComponente>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline MascaraComponente& operator&=(MascaraComponente& lhs, MascaraComponente rhs) {
	lhs = lhs & rhs;
	return lhs;
}

inline MascaraComponente operator~(MascaraComponente mask) {
	using T = std::underlying_type_t<MascaraComponente>;
	return static_cast<MascaraComponente>(~static_cast<T>(mask));
}

struct Componente
{
	virtual ~Componente() = default; 
};