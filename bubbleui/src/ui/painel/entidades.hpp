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
			Entidades(std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4<int> &rect = { 2, 2, 100, 50 });
			void recarregar();
			void definirContexto(std::shared_ptr < Contexto >) override;
		private:
			std::shared_ptr<Bubble::Cena::SceneManager> scenemanager;
			int quantidade_entidades{ 0 }, quantidade_cenas{ 0 };
			void recursivo(std::shared_ptr<Bubble::Entidades::Entidade> entidade, std::shared_ptr < Widgets::Arvore> arvore);
			void preAtualizacao() override;
		};
	}
}