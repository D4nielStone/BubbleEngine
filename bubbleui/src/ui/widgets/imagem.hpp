#pragma once
#include "widget.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Imagem : public Widget
		{
		public:
			Imagem(unsigned int id, Vector2 size = {100, 100}, bool auto_resize = true);
			void atualizar(float deltaTime) override;	
			void renderizar() override;
			void defID(unsigned int newID);
			Vector4 obtRect() const;
		private:
			unsigned int ID;
			Vector2 tamanho;
			bool preencher;
		};
	}
}