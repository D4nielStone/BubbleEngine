#pragma once
#include "painel.hpp"
#include "src/ui/widgets/imagem.hpp"
#include "src/cena/scenemanager.hpp"
#include <src/ui/widgets/imagem.hpp>

namespace BubbleUI
{
	namespace Paineis {
		class BEUI_DLL_API Editor : public Painel
		{
		public:
			Editor(std::shared_ptr < Contexto>ctx, std::shared_ptr < Bubble::Cena::SceneManager> scenemanager, const Vector4 &rect = {2, 2, 50, 50});
			void preAtualizacao() override;
		private: 
			std::shared_ptr<Bubble::Cena::SceneManager> scenemanager;
			std::shared_ptr<Widgets::Imagem> buffer;
			Vector2 posicao_da_camera;
			glm::vec3 pos_antiga;
		};
	}
}