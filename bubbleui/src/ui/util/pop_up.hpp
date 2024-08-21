#include "src/ui/formas/moldura.hpp"

namespace BubbleUI
{
	namespace Util
	{
		class BEUI_DLL_API PopUp : public Formas::Moldura
		{
		public:
			PopUp(Contexto*);
		private:
			void preAtualizacao() override;
			Bubble::Inputs::Inputs* inputs{ nullptr };
		};
	}
}