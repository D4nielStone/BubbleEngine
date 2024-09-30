#include "src/ui/formas/moldura.hpp"
#include "src/ui/formas/colisao2d.hpp"
#include <memory>

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
			PopUp(std::shared_ptr<Contexto>);
			void mostrar();
			void esconder();
			void renderizar() const override;
			void adiItem(std::shared_ptr<Items::ItemMenu> item);
			bool temItems() const;
			bool escondido() const;
			bool mouseEmCima{ false };
		private:
			int largura{ 0 }, altura{ 0 };
			void posAtualizacao() override;
			bool vmostrar{ false }, podeEsconder{ false };
			Colisao2d colisao;
			std::shared_ptr<Bubble::Inputs::Inputs> inputs{ nullptr };
			std::vector<std::shared_ptr<Items::ItemMenu>> lista_items;
		};
	}
}