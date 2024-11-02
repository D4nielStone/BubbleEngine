#pragma once
#include "src/ui/formas/moldura.hpp"
#include "src/ui/widgets/widget.hpp"
#include "src/ui/util/pop_up.hpp"
#include "separador.hpp"
#include "bubbleui.hpp"
#include "aba.hpp"
#include <memory>
#include <vector>

namespace BubbleUI
{
	class BEUI_DLL_API Painel
	{
	public:
		// Construtores
		Painel() = default;
		Painel(std::shared_ptr<Contexto> ctx, const Vector4& rect = {0, 0, 100, 50});

		// Manipulação de Tamanho e Posição
		void definirTamanho(const Vector2& tam);
		void definirPosicao(const Vector2& pos);
		void adicionarTamanho(const Vector2& tam);
		void adicionarPosicao(const Vector2& pos);

		// Widgets
		void adicionarWidget(std::shared_ptr<Widget> widget);

		// Obtenção de Dados
		Vector4 obterRetangulo() const;
		Vector2 obterTamanhoMinimo() const;
		std::shared_ptr<Contexto> obterContexto() const;

		// Atualização e Renderização
		void atualizar();
		void renderizar() const;

		// Estado do Painel
		bool cursorNormal() const;
		std::string nome() const { return Nome; }

		// Estado Interno
		Vector2 widgetPadding, posicaoWidget;
		Color arvoreCor{ 0.1f, 0.1f, 0.1f };

		// Flags de Controle
		Lado redimensionamentoAtual;
		bool selecionado{ false };
		bool arrastando{ false };
		bool mouse1click{ false }, redimensionavel{ true }, mostrar_aba{ true };
	protected:
		// Métodos de Configuração e Ciclo de Vida
		void configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect = { 2, 2, 100, 50 });
		virtual void preAtualizacao() {}
		virtual void posAtualizacao() {}
		virtual void preRenderizacao() const {}
		virtual void posRenderizacao() const {}

		void corrigirLimite();

		// Atributos
		std::string Nome = "Painel";
		Formas::Moldura moldura;
		std::shared_ptr<Contexto> contexto{ nullptr };
		std::vector<std::shared_ptr<Widget>> lista_widgets;

		// Componentes
		std::unique_ptr<Util::PopUp> menuDeContexto{ nullptr };
		std::unique_ptr<Separador> bordaEsq, bordaDir, bordaCima, bordaBaixo;
		std::unique_ptr<Aba> aba{ nullptr };

		// Dados de Geometria
		Vector4 retangulo;
		Vector2 tamanhoMinimo;
		bool renderizarCorpo{ true };

	};

} // namespace BubbleUI