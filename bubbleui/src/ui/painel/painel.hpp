
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#pragma once
#include "src/ui/formas/moldura.hpp"
#include "src/ui/widgets/widget.hpp"
#include "src/ui/util/pop_up.hpp"
#include "separador.hpp"
#include "bubbleui.hpp"
#include "memory"
#include "aba.hpp"
#include <memory>
#include <vector>

namespace BubbleUI
{
	class BEUI_DLL_API Painel : public Formas::Moldura
	{
	public:
		// Construtores
		Painel(const Vector4& rect);
		Painel(const char* n, const Vector4& rect);
		Painel() = default;

		// Widgets
		void adicionarWidget(std::shared_ptr<Widget> widget);

		// Obtenção de Dados
		Vector2 obterTamanhoMinimo() const;
		std::shared_ptr<Contexto> obterContexto() const;

		// Atualização e Renderização
		void atualizar() override;
		void renderizar() override;

		// Estado do Painel
		bool cursorNormal() const;
		std::string nome() const { return Nome; }
		virtual void definirContexto(std::shared_ptr<Contexto>, const char* nome, const Vector4& rect);
		virtual void definirContexto(std::shared_ptr<Contexto>t);
		void Fullscreen(const bool& booleano);
		// Flags de Controle
		Lado redimensionamentoAtual;
		bool arrastando{ false };
		bool mouse1click{ false }, redimensionavel{ true }, mostrar_aba{ true };
		std::vector<std::shared_ptr<Widget>> widgets() const;

		void defCor(const Color& cor) override;
	protected:
		// Métodos de Configuração e Ciclo de Vida
		void configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect = { 2, 2, 200, 100 });
		virtual void preAtualizacao() {}
		virtual void posRenderizacao() const {}
		virtual void posAtualizacao() {}
		virtual void preRenderizacao() const {}

		void corrigirLimite();

		// Atributos
		std::string Nome = "Painel";
		std::vector<std::shared_ptr<Widget>> lista_widgets;

		// Componentes
		std::unique_ptr<Util::PopUp> menuDeContexto{ nullptr };
		std::unique_ptr<Separador> bordaEsq, bordaDir, bordaCima, bordaBaixo;
		std::unique_ptr<Aba> aba{ nullptr };

		// Dados de Geometria
		Vector2 tamanhoMinimo{200, 100};
		bool renderizarCorpo{ true };
		bool preencher{};   // flag para preencher a tela com o painel
	};

} // namespace BubbleUI