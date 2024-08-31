#pragma once
#include "painel.hpp"
#include "src/ui/widgets/imagem.hpp"
#include "src/cena/scenemanager.hpp"
#include "src/ui/widgets/arvore.hpp"
#include <src/ui/widgets/imagem.hpp>

namespace BubbleUI
{
	namespace Paineis {
		class BEUI_DLL_API Entidades : public Painel
		{
		public:
			Entidades(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect = { 2, 2, 100, 50 });
			void recarregar();
			void preAtualizacao() override;
		private:
			bool gatilho_recarregar{ false };
			Bubble::Cena::SceneManager* scenemanager;
			void recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade, Widgets::Arvore& arvore);
		};
	}
}