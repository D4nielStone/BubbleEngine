/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file entidade.hpp
 * @brief Inclusoes e definições relacionadas à entidade
 */

#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include "src/componentes/componente.hpp"
#include "src/depuracao/debug.hpp"

/* Definição de Entidade */
#include <functional>

// Exemplo de uma estrutura Entidade
struct Entidade {
	uint32_t id;
	MascaraComponente mascara = COMPONENTE_NONE;

	bool operator==(const Entidade& other) const {
		return id == other.id;
	}
};

// Implementação de std::hash para Entidade
namespace std {
	template <>
	struct hash<Entidade> {
		std::size_t operator()(const Entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}

/* Gerenciador ECS */
class GerenciadorDeEntidades
{
private:
	uint32_t proxima_entidade{ 0 };
	/// Armazena mascara da entidade associada
	std::unordered_map<uint32_t, MascaraComponente> mascaras;
	/// Armazena componentes da entidade associada
	std::unordered_map<uint32_t, std::unordered_map<MascaraComponente, std::shared_ptr<Componente>>> entidades;
public:
	/* Cria nova entidade */
	Entidade criarEntidade();
	/* Adiciona um componente a uma entidade */
	template <typename T, typename... Args>
	void adicionarComponente(Entidade& ent, Args&&... args);

	/* Verifica se uma entidade possui um componente */
	template <typename T>
	bool temComponent(const uint32_t& entity);

	/* Itera pelas entidades que possuem determinados componentes */
	template <typename... Components, typename Func>
	void paraCadaEntidade(Func func);

	// Obtém um componente de uma entidade
	template <typename T>
	std::shared_ptr<T> obterComponete(const uint32_t& entity);
};

/* Definições de templates */

template<typename T, typename ...Args>
void GerenciadorDeEntidades::adicionarComponente(Entidade& ent, Args&&... args) {
	ent.mascara |= T::mascara;
	mascaras[ent.id] = ent.mascara; // Atualiza a máscara no mapa auxiliar
	entidades[ent.id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
	Debug::emitir("GenEnt", "Componente adicionado para: " + std::to_string(ent.id) +
		" Componente: " + std::to_string(T::mascara));
	Debug::emitir("GenEnt", "Máscara final para " + std::to_string(ent.id) +
		" = " + std::to_string(mascaras[ent.id]));
}


template<typename T>
inline bool GerenciadorDeEntidades::temComponent(const uint32_t& entity)
{
	return (mascaras[entity] & T::mascara) != 0;
}

template<typename ...Componentes, typename Func>
inline void GerenciadorDeEntidades::paraCadaEntidade(Func func)
{
	for (auto& [entity, comps] : entidades) {
		if ((temComponent<Componentes>(entity) && ...)) {
			func(entity);
		}
	}
}

template<typename T>
inline std::shared_ptr<T> GerenciadorDeEntidades::obterComponete(const uint32_t &entity)
{
	auto it = entidades[entity].find(T::mascara);
	if (it != entidades[entity].end()) {
		return std::static_pointer_cast<T>(it->second);
	}
	return nullptr;
}
