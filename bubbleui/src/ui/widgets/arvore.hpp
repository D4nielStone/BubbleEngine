#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Arvore : public Texto
		{
		public:
			Arvore(std::string label, bool* retorno);
			void atualizar() override;
			void renderizar() override;
			void defPainel(Painel* painel) override;
			void adiFilho(std::shared_ptr<Arvore> filho);
			Arvore* arvore_pai{ nullptr };
		private:
			std::vector<std::shared_ptr<Arvore>> filhos;
			Color cor;
			bool aberto{ true }, * retorno{ nullptr };
		};
	}
}