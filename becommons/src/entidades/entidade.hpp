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
using Entidade = std::size_t;

/* Gerenciador ECS */
class GerenciadorDeEntidades
{
private:
	Entidade proxima_entidade{ 0 };
	std::unordered_map<Entidade, std::unordered_map<std::type_index, std::shared_ptr<Componente>>> entidades;
public:
	/* Cria nova entidade */
	Entidade criarEntidade();
	/* Adiciona um componente a uma entidade */
	template <typename T, typename... Args>
	void adicionarComponente(const Entidade& ent, Args&&... args);

	/* Verifica se uma entidade possui um componente */
	template <typename T>
	bool temComponent(const Entidade& entity);

	/* Itera pelas entidades que possuem determinados componentes */
	template <typename... Components, typename Func>
	void paraCadaEntidade(Func func);

	// Obtém um componente de uma entidade
	template <typename T>
	std::shared_ptr<T> obterComponete(Entidade entity);
};

/* Definições de templates */

template<typename T, typename ...Args>
inline void GerenciadorDeEntidades::adicionarComponente(const Entidade& ent, Args && ...args)
{
	entidades[ent][std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
	Debug::emitir("GenEnt", "componente adicionado para: " + std::to_string(ent));
}

template<typename T>
inline bool GerenciadorDeEntidades::temComponent(const Entidade& entity)
{
	return entidades[entity].count(std::type_index(typeid(T))) > 0;
}

template<typename ...Componentes, typename Func>
inline void GerenciadorDeEntidades::paraCadaEntidade(Func func)
{
	for (auto& [entity, components] : entidades) {
		if ((temComponent<Componentes>(entity) && ...)) {
			func(entity);
		}
	}
}

template<typename T>
inline std::shared_ptr<T> GerenciadorDeEntidades::obterComponete(Entidade entity)
{
	auto it = entidades[entity].find(std::type_index(typeid(T)));
	if (it != entidades[entity].end()) {
		return std::static_pointer_cast<T>(it->second);
	}
	return nullptr;
}
