/** @copyright Copyright (c) 2024 Daniel Oliveira */
/** @file painel.hpp 
 *	@brief classe painel, o container pra widgets e etc...
 * 
*/

#pragma once
#include "src/ui/formas/moldura.hpp"
#include "src/ui/widgets/widget.hpp"
#include "src/ui/util/pop_up.hpp"
#include "bubbleui.hpp"
#include "memory"
#include "aba.hpp"
#include <vector>

namespace bubbleui
{
	class painel : public Formas::Moldura
	{
	public:
		// Construtores
		painel(const bubble::vetor4<int>& rect);
		painel(const char* n, const bubble::vetor4<int>& rect);
		painel() = default;

		// Widgets
		void adicionarWidget(std::shared_ptr<Widget> widget);

		// Obtenção de Dados
		std::shared_ptr<Contexto> obterContexto() const;

		// Atualização e Renderização
		void atualizar() override;
		void renderizar() override;

		// Estado do Painel
		std::string nome() const { return Nome; }
		virtual void definirContexto(std::shared_ptr<Contexto>, const char* nome, const bubble::vetor4<int>& rect);
		virtual void definirContexto(std::shared_ptr<Contexto>);
		// Flags de Controle
		bool mouse1click{ false }, mostrar_aba{ true };
		std::vector<std::shared_ptr<Widget>> widgets() const;

		void defCor(const bubble::cor& cor) override;
	protected:
		std::shared_ptr<Colisao2d> colisao = std::make_shared<Colisao2d>();
		// Métodos de Configuração e Ciclo de Vida
		void configurar(std::shared_ptr<Contexto> ctx, const bubble::vetor4<int>& rect = { 2, 2, 200, 100 });
		virtual void preAtualizacao() {}
		virtual void posRenderizacao() const {}
		virtual void posAtualizacao() {}
		virtual void preRenderizacao() const {}

		// Atributos
		std::string Nome = "Painel";
		std::vector<std::shared_ptr<Widget>> lista_widgets;

		// Componentes
		std::unique_ptr<Util::PopUp> menuDeContexto{ nullptr };
		std::unique_ptr<Aba> aba{ nullptr };

		// Dados de Geometria
		bool renderizarCorpo{ true };
		bool ativar_mc = false;
	};

}