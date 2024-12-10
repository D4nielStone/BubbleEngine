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
};

template<typename T, typename ...Args>
inline void GerenciadorDeEntidades::adicionarComponente(const Entidade& ent, Args && ...args)
{
	entidades[ent][std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
	Debug::emitir("GenEnt", "componente adicionado para: " + std::to_string(ent));
}