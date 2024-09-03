#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Botao : public Texto
		{
		public:
			Botao(const std::string &label);
			void atualizar() override;
			void renderizar() const override;
		private:
		};
	}
}