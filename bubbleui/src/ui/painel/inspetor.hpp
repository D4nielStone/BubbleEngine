#pragma once
#include "painel.hpp"
#include "src/cena/scenemanager.hpp"

namespace BubbleUI
{
	namespace Paineis {
		class BEUI_DLL_API Inspetor : public Painel
		{
		public:
			Inspetor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4 &rect = { 2, 2, 100, 50 });
			~Inspetor();
		private:
			void recarregar();
			void preAtualizacao() override;
			std::shared_ptr<Bubble::Entidades::Entidade> entidade_selecionada{ nullptr };
			std::shared_ptr<Bubble::Cena::SceneManager> scenemanager{ nullptr };
			std::shared_ptr<std::string> nome_atual{ nullptr };
		};
	}
}