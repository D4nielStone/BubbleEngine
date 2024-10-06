#include "imagem.hpp"
#include "texto.hpp"
#include "src/ui/painel/separador.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API SeletorDeCor : public Texto
		{
		public:
			SeletorDeCor(Color* cor, const std::string& label, const Alinhamento& alinhamento = Alinhamento::Centro);
			void atualizar() override;
			void renderizar() const override;
			void defPainel(Painel* painel) override;
		private:
			Color* cor_callback{ nullptr };
			int tamanho{ 96 };
			std::unique_ptr<Imagem> arco_cor{ nullptr };
		};
	}
}