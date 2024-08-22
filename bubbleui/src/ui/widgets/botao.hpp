#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Botao : public Texto
		{
		public:
			Botao(std::string label);
			void atualizar(float deltaTime) override;
			void renderizar() override;
		private:
		};
	}
}