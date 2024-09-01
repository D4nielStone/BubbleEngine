#pragma once
#include "painel.hpp"
#include "src/cena/scenemanager.hpp"

namespace BubbleUI
{
	namespace Paineis {
		class BEUI_DLL_API Inspetor : public Painel
		{
		public:
			Inspetor(Contexto* ctx, Bubble::Cena::SceneManager* scenemanager, Vector4 rect = { 2, 2, 100, 50 });
			~Inspetor();
		private:
			void recarregar();
			void preAtualizacao() override;
			Bubble::Entidades::Entidade* entidade_selecionada{ nullptr };
			Bubble::Cena::SceneManager* scenemanager;
			std::string* nome_atual;
		};
	}
}