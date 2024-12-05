#pragma once
#include "texto.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API CheckBox : public Texto
		{
		public:
			CheckBox(bool *retorno, const std::string &texto = "", const Alinhamento& alinhamento = Alinhamento::Esquerda);
			void atualizar() override;
			void renderizar() override;
		private:
			Bubble::Arquivadores::Shader shaderImg = Bubble::Arquivadores::Shader("assets/shaders/imagem.vert", "assets/shaders/imagem.frag");
			void renderizarImg();
			Vector4<float> rect_ndc;
			bool* retorno{ nullptr }, gatilho{ false }, deveRenderizar{ true };
			int size{ 16 }; unsigned int IDimagem;
		};
	}
}