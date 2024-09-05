#pragma once
#include "src/ui/formas/moldura.hpp"
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
		};
	}
}