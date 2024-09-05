#pragma once
#include "src/ui/painel/painel.hpp"
#include "src/ui/contexto/contexto.hpp"
#include "src/ui/util/barra_menu.hpp"

namespace Bubble
{
	namespace Nucleo
	{
		class Engine;
	}
}
namespace BubbleUI
{
	class Manager
	{
	private:
		std::vector<std::shared_ptr<Painel>> lista_paineis;
		Util::BarraMenu barra_de_menu;
		Bubble::Nucleo::Engine* engine = nullptr;
		std::shared_ptr<Contexto> contexto{ nullptr };
		Colisao2d colisao_painel;
		void iniPaineisPadrao();
		void painelSelecionado(std::shared_ptr<Painel> painel);
	public:
		GLFWcursor* cursor_antigo{ nullptr };
		Manager(Bubble::Nucleo::Engine*);
		void atualizar();
		void verificarSelecionado();
		void renderizar() const;
	};
}
