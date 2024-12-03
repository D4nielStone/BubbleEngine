#pragma once
#include "item_menu.hpp"
#include "src/ui/widgets/imagem.hpp"

namespace BubbleUI
{
	namespace Items
	{
		class BEUI_DLL_API Botao : public ItemMenu
		{

		public:
			Botao() = default;
			Botao(const std::string &label_shared, bool* callback);
			Botao(const std::string &label_shared, bool* callback, const std::string &imagePath);
			void atualizar() override;
			void renderizar() override;
		private:
			bool* callback{ nullptr };
			std::unique_ptr<Widgets::Imagem> icon{nullptr};
		};
	}
}