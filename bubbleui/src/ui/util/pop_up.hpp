#include "src/ui/formas/moldura.hpp"

namespace BubbleUI
{
	namespace Util
	{
		class BEUI_DLL_API PopUp : public Formas::Moldura
		{
		public:
			PopUp(Contexto*);
			void mostrar();
			void esconder();
			void renderizar(GLenum) override;
		private:
			void preAtualizacao() override;
			bool vmostrar{ false };
			Bubble::Inputs::Inputs* inputs{ nullptr };
		};
	}
}