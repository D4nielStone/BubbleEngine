#include "src/ui/formas/moldura.hpp"

namespace BubbleUI
{
	namespace Items
	{
		class ItemMenu;
	}
	namespace Util
	{
		class BEUI_DLL_API PopUp : public Formas::Moldura
		{
		public:
			PopUp(Contexto*);
			void mostrar();
			void esconder();
			void renderizar(GLenum) override;
			void adiItem(Items::ItemMenu* item);
		private:
			int largura{ 0 }, altura{ 0 };
			void preAtualizacao() override;
			bool vmostrar{ false };
			Bubble::Inputs::Inputs* inputs{ nullptr };
			std::vector<Items::ItemMenu*> lista_items;
		};
	}
}