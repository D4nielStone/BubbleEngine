#pragma once
#include "src/ui/formas/rect.hpp"
#include "src/ui/contexto/contexto.hpp"

namespace Bubble
{
	namespace Nucleo
	{
		class Engine;
	}
}
namespace BubbleUI
{
	class Manager
	{
	private:
		Formas::Rect* retangulotest = nullptr;
		Bubble::Nucleo::Engine* engine = nullptr;
		Contexto contexto;
		void iniPaineisPadrao();
	public:
		Manager(Bubble::Nucleo::Engine*);
		void atualizar(float deltaTime);
		void renderizar();
	};
}
