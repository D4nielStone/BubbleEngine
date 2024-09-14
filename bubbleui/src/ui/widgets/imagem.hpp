#pragma once
#include "widget.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Imagem : public Widget
		{
		public:
			Imagem(unsigned int id, const Vector2 &size = {100, 100}, const bool &auto_resize = true);
			Imagem(const std::string& path, int size_percentage, Vector2* posicao = nullptr);
			Vector4f paraNDC() const;
			void atualizar() override;
			void renderizar() const override;
			void defID(unsigned int newID);
			Vector4 obtRect() const;
			bool deveRenderizar{ true };
		private:
			unsigned int ID;
			Vector2*posicao_ptr{ nullptr };
			Vector4 rect;
			Shader shader = Shader("assets/shaders/imagem.vert", "assets/shaders/imagem.frag");
			bool preencher{ false };
		};
	}
}