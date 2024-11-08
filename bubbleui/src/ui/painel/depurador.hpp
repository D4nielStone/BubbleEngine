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
			Depurador(std::shared_ptr<Contexto> ctx, const Vector4& retangulo = {0, 31, 100, 50});
		private:
			void preAtualizacao() override;
			double fps = 0.0, elapsedTime = 0.0, lastTime = 0.0;
			int frames = 0;
			std::string fps_display;
		};
	}
}