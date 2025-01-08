/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file entidade.hpp
 * @brief Inclusoes e definições relacionadas à entidade
 */

#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "src/componentes/componente.hpp"
#include "src/componentes/fisica.hpp"
#include "src/depuracao/debug.hpp"

namespace bubble
{
	/**
	 * @struct entidade
	 * @brief todo elemento do faserio
	 */
	struct entidade {
		uint32_t id;
		componente::mascara mascara = componente::COMPONENTE_NONE;

		bool operator==(const entidade& other) const {
			return id == other.id;
		}
	};

	/**
	* @struct registro
	* @brief gerencia as entidades
	* @brief nucleo do sistema ECS
	*/
	class registro
	{
	private:
		/// proximo id livre
		uint32_t proxima_entidade{ 0 };
		/// Armazena mascara da entidade associada
		std::unordered_map<uint32_t, bubble::componente::mascara> mascaras;
		/// Armazena componentes da entidade associada
		std::unordered_map<uint32_t, std::unordered_map<bubble::componente::mascara, std::shared_ptr<bubble::componente>>> entidades;
	public:
		/* Cria nova entidade */
		entidade criar();
		/* Adiciona um componente a uma entidade */
		template <typename T, typename... Args>
		void adicionar(entidade& ent, Args&&... args);

		/* Verifica se uma entidade possui um componente */
		template <typename T>
		bool tem(const uint32_t& entity);

		/* Itera pelas entidades que possuem determinados componentes */
		template <typename... Components, typename Func>
		void cada(Func func);

		/* Obtem um componente de uma entidade */
		template <typename T>
		std::shared_ptr<T> obter(const uint32_t& entity);
	};

	/* Definições de templates */

	template<typename T, typename ...Args>
	void registro::adicionar(entidade& ent, Args&&... args) {
		ent.mascara |= T::mascara;
		mascaras[ent.id] = ent.mascara; // Atualiza a máscara no mapa auxiliar
		entidades[ent.id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
		entidades[ent.id][T::mascara]->meu_objeto = ent.id;
	}

	template<typename T>
	inline bool registro::tem(const uint32_t& entity)
	{
		return (mascaras[entity] & T::mascara) != 0;
	}

	template<typename ...componentes, typename Func>
	inline void registro::cada(Func func)
	{
		for (auto& [entity, comps] : entidades) {
			if ((tem<componentes>(entity) && ...)) {
				func(entity);
			}
		}
	}

	template<typename T>
	inline std::shared_ptr<T> registro::obter(const uint32_t& entity)
	{
		auto it = entidades[entity].find(T::mascara);
		return std::static_pointer_cast<T>(it->second);
	}

}	///< namespace bubble

// Implementação de std::hash para entidade
namespace std {
	template <>
	struct hash<bubble::entidade> {
		std::size_t operator()(const bubble::entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}