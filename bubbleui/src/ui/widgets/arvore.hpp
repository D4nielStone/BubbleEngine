#pragma once
#include "imagem.hpp"
#include "texto.hpp"
namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Arvore : public Texto, public std::enable_shared_from_this<Arvore>
		{
		public:
			Arvore(const std::string& label_shared, bool* retorno);
			Arvore(const std::string& label_shared, bool* retorno, const std::string &imagePath);
			Arvore(std::shared_ptr<std::string>label_shared, bool* retorno);
			Arvore(std::shared_ptr<std::string>label_shared, bool* retorno, const std::string &imagePath);
			void atualizar() override;
			void renderizar() const override;
			void defPainel(Painel* painel) override;
			void adiFilho(std::shared_ptr<Widget> filho);
		private:
			std::vector<std::shared_ptr<Widget>> filhos;
			Color cor;
			std::shared_ptr<std::string> label_shared;
			std::unique_ptr<Imagem> icone{ nullptr };
			bool aberto{ false }, * retorno{ nullptr }, gatilho_click{ false };
		};
	}
}