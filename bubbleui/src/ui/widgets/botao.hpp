#include "texto.hpp"
#include "imagem.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Botao : public Texto
		{
		public:
			Botao(const std::string &label_shared, std::function<void()> funcao_click, const std::string& imagePath,bool completo = true);
			Botao(const std::string &label_shared, std::function<void()> funcao_click, bool completo = true);
			void atualizar() override;
			void renderizar() const override;
		private:
			bool completo{}, gatilho{ false };
			std::function<void()> funcao_click_;
			std::unique_ptr<Imagem> icon{ nullptr };
		};
	}
}