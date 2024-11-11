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
	class BEUI_DLL_API Painel : public Formas::Moldura
	{
	public:
		// Construtores
		Painel(const Vector4& rect);
		Painel() = default;

		// Widgets
		void adicionarWidget(std::shared_ptr<Widget> widget);

		// Obtenção de Dados
		Vector2 obterTamanhoMinimo() const;
		std::shared_ptr<Contexto> obterContexto() const;

		// Atualização e Renderização
		void atualizar() override;
		void renderizar() const override;

		// Estado do Painel
		bool cursorNormal() const;
		std::string nome() const { return Nome; }
		virtual void definirContexto(std::shared_ptr<Contexto>);

		// Flags de Controle
		Lado redimensionamentoAtual;
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
		std::vector<std::shared_ptr<Widget>> lista_widgets;

		// Componentes
		std::unique_ptr<Util::PopUp> menuDeContexto{ nullptr };
		std::unique_ptr<Separador> bordaEsq, bordaDir, bordaCima, bordaBaixo;
		std::unique_ptr<Aba> aba{ nullptr };

		// Dados de Geometria
		Vector2 tamanhoMinimo{100, 50};
		bool renderizarCorpo{ true };

	};

} // namespace BubbleUI