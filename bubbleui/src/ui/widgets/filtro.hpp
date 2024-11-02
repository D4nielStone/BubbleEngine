#pragma once
#include "widget.hpp"
#include "botao.hpp"

namespace BubbleUI 
{
	namespace Widgets
	{
		class BEUI_DLL_API Filtro : public Widget
		{
		public:
			Filtro(const std::string &label);
			void atualizar() override;
			void renderizar() const override;
		private:
		};
	}
}