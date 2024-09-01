#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"

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
			void adiItem(std::shared_ptr<Items::ItemMenu> item);
			bool mouseEmCima{ false };
		private:
			int largura{ 0 }, altura{ 0 };
			void posAtualizacao() override;
			bool vmostrar{ false };
			Bubble::Inputs::Inputs* inputs{ nullptr };
			Colisao2d* colisao{ nullptr };
			std::vector<std::shared_ptr<Items::ItemMenu>> lista_items;
		};
	}
}