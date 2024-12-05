#pragma once
#include "painel.hpp"
#include <memory>

namespace BubbleUI
{
	namespace Paineis
	{
		class BEUI_DLL_API Depurador : public Painel
		{
		public:
			Depurador(const Vector4<int>& retangulo = {0, 31, 100, 50});
			void definirContexto(std::shared_ptr<Contexto> ctx) override;
		private:
			void preAtualizacao() override;
			double fps = 0.0, elapsedTime = 0.0, lastTime = 0.0;
			int frames = 0;
			std::shared_ptr<Widgets::Texto >fps_display;
		};
	}
}