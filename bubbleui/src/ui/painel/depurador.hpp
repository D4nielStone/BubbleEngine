#pragma once
#include "painel.hpp"

namespace BubbleUI
{
	namespace Paineis
	{
		class BEUI_DLL_API Depurador : public Painel
		{
		public:
			Depurador(Contexto* ctx);
		private:
			void preAtualizacao() override;
			double fps = 0.0, elapsedTime = 0.0, lastTime = 0.0;
			int frames = 0;
			std::string fps_display;
		};
	}
}