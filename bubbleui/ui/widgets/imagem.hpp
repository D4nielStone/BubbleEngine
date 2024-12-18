#pragma once
#include "widget.hpp"

namespace BubbleUI
{
	namespace Widgets
	{
		class BEUI_DLL_API Imagem : public Widget
		{
		public:
			Imagem() = default;
			Imagem(unsigned int id, const Vector2& size = { 100, 100 }, const bool& auto_resize = true);
			Imagem(const std::string& path, int size_percentage, Vector2* posicao = nullptr);
			Imagem(const std::string& path, const Vector2& size = {16, 16});
			bubble::vetor4<float> paraNDC() const;
			void atualizar() override;
			void renderizar() override;
			bool defID(unsigned int newID);
			bubble::vetor4<int> obtRect() const;
			bool deveRenderizar{ true }, flip{ false }, padding{ false };
		private:
			unsigned int ID;
			Vector2* posicao_ptr{ nullptr };
			bubble::vetor4<int> rect;
			Vector2 tamanho_original{};
			std::string path;
			unsigned int ip;
			Bubble::Arquivadores::Shader shader = Bubble::Arquivadores::Shader("assets/shaders/imagem.vert", "assets/shaders/imagem.frag");
			bool preencher{ false };
		};
	}
}