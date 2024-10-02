#pragma once
#include "src/ui/painel/painel.hpp"
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/util/barra_menu.hpp"

namespace Bubble
{
	namespace Nucleo
	{
		class Projeto;
	}
}
namespace BubbleUI
{
	class Manager
	{
	private:
		std::vector<std::shared_ptr<Painel>> lista_paineis;
		Util::BarraMenu barra_de_menu;
		Bubble::Nucleo::Projeto* projeto{ nullptr };
		std::shared_ptr<Contexto> contexto{ nullptr };
		Colisao2d colisao_painel;
		void iniPaineisPadrao();
		void painelSelecionado(std::shared_ptr<Painel> painel);
	public:
		GLFWcursor* cursor_antigo{ nullptr };
		Manager(Bubble::Nucleo::Projeto*);
		void atualizar();
		void verificarSelecionado();
		void renderizar() const;
	};
}
