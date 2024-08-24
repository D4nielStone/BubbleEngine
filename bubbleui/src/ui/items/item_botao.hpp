#include "item_menu.hpp"
#include <functional>

namespace BubbleUI
{
	namespace Items
	{
		class BEUI_DLL_API Botao : public ItemMenu
		{

		public:
			Botao() = default;
			Botao(std::string label, std::function<void()> funcao_click);
			void atualizar() override;
		private:
			std::function<void()> funcao_click_;
		};
	}
}