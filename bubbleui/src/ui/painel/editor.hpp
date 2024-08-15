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
			Editor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect = {2, 2, 50, 50});
			void preAtualizacao() override;
		private: 
			Bubble::Cena::SceneManager* scenemanager;
			Widgets::Imagem* buffer;
		};
	}
}