#include "item_menu.hpp"

namespace BubbleUI
{
	namespace Util
	{
		class PopUp;
	}
	namespace Items
	{
		class BEUI_DLL_API Arvore : public ItemMenu
		{

		public:
			Arvore() = default;
			Arvore(std::string label, Util::PopUp* popup);
			void atualizar() override;
			void renderizar() override;
		private:
			Util::PopUp* m_popup{ nullptr };
		};
	}
}