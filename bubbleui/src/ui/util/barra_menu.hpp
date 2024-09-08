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
			BarraMenu() = default;
			BarraMenu(std::shared_ptr<Contexto> contexto);
			void renderizar() const;
			void atualizar();
		private:
			int altura{ 30 };
			std::unique_ptr<Items::ItemMenu> arvoreCena;
			std::shared_ptr<Util::PopUp> popupCena{ nullptr };
		};
	}
}