#pragma once
#include "src/ui/widgets/texto.hpp"
#include "src/ui/formas/moldura.hpp"
#include "src/ui/items/item_arvore.hpp"
namespace BubbleUI
{
	namespace Util
	{
		class BEUI_DLL_API BarraMenu : public Formas::Moldura
		{
		public:
			BarraMenu() {};
			void renderizar() const;
			void salvarCenaAtual() const;
			void atualizar();
			void adicionarBotao(std::unique_ptr<Items::ItemMenu> item);
			void defContexto(std::shared_ptr<Contexto> ctx);
		private:
			int altura{ 22 };
			std::vector<std::unique_ptr<Items::ItemMenu>> botoes;
			Widgets::Texto texto_nome_projeto;
			std::shared_ptr<Util::PopUp> popupCena{ nullptr };
			bool callbackSalvarCenaAtual{ false };
		};
	}
}