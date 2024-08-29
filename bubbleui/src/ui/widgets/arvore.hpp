#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Arvore : public Texto
		{
		public:
			Arvore(std::string label);
			void atualizar(float deltaTime) override;
			void renderizar() override;
		private:
		};
	}
}