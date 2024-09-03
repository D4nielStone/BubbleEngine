#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Arvore : public Texto, public std::enable_shared_from_this<Arvore>
		{
		public:
			Arvore(std::string label, bool* retorno);
			void atualizar() override;
			void renderizar() const override;
			void defPainel(Painel* painel) override;
			void adiFilho(std::shared_ptr<Arvore> filho);
			std::shared_ptr<Arvore> arvore_pai{ nullptr };
		private:
			std::vector<std::shared_ptr<Arvore>> filhos;
			Color cor;
			bool aberto{ false }, * retorno{ nullptr }, gatilho_click{ false };
		};
	}
}