#pragma once
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
			BarraMenu(std::shared_ptr<Contexto> contexto);
			void renderizar() const;
			void atualizar();
			void adicionarBotao(std::unique_ptr<Items::ItemMenu> item);
			void defContexto(std::shared_ptr<Contexto> ctx);
		private:
			int altura{ 26 };
			std::vector<std::unique_ptr<Items::ItemMenu>> botoes;
			std::shared_ptr<Util::PopUp> popupCena{ nullptr };
		};
	}
}