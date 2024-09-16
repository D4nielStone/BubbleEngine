#include "widget.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API CheckBox : public Widget
		{
		public:
			CheckBox(bool *retorno);
			void atualizar() override;
			void renderizar() const override;
		private:
			Shader shaderImg = Shader("assets/shaders/imagem.vert", "assets/shaders/imagem.frag");
			void renderizarImg() const;
			bool* retorno{ nullptr }, gatilho{ false }, deveRenderizar{ true };
			int size{ 22 }; unsigned int IDimagem;
		};
	}
}