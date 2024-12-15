/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include <GLFW/glfw3.h>
#include "src/util/vetor2.hpp"
#include "src/nucleo/sistema.hpp"
#include "src/nucleo/sistema_de_renderizacao.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

struct Janela
{
	Vetor2<int> tamanho;
	std::unordered_map<std::type_index, std::shared_ptr<Sistema>> sistemas;
	std::unordered_map<uint32_t, std::function<void(uint32_t)>> tarefas;
	GLFWwindow* window;
	Janela(const char* nome);
	template<typename T>
	void adicionarSistema(GerenciadorDeEntidades* ge);
	void adicionarTarefa(const uint32_t& id, std::function<void(uint32_t)> funcao);
	void iniciarLoop() const;
};

template<typename T>
inline void Janela::adicionarSistema(GerenciadorDeEntidades* ge)
{
	auto sys = std::make_shared<T>(ge);
	sys->inicializar();
	sistemas[std::type_index(typeid(T))] = sys;
}