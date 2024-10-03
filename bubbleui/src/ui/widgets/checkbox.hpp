#pragma once
#include "texto.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API CheckBox : public Texto
		{
		public:
			CheckBox(bool *retorno, const std::string &texto = "");
			void atualizar() override;
			void renderizar() const override;
		private:
			Shader shaderImg = Shader("assets/shaders/imagem.vert", "assets/shaders/imagem.frag");
			void renderizarImg() const;
			bool* retorno{ nullptr }, gatilho{ false }, deveRenderizar{ true };
			int size{ 16 }; unsigned int IDimagem;
		};
	}
}