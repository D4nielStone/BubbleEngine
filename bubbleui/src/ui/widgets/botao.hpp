#pragma once
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
			Botao(const std::string &label_shared, const std::string& imagePath, bool* callback = nullptr, bool completo = true);
			Botao(const std::string &label_shared, bool* callback = nullptr, bool completo = true);
			Botao() = default;
			void definirPai(Moldura* pai) override;
			void atualizar() override;
			void renderizar() const override;
			int _Meu_iconsize{ 15 };
		private:
			bool completo{}, gatilho{ false };
			bool* callback{ nullptr };
			std::function<void()> funcao_click_;
			std::unique_ptr<Imagem> icon{ nullptr };
		};
	}
}