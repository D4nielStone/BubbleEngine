#pragma once
#include "src/ui/contexto/contexto.hpp"

namespace BubbleUI
{
	class Colisao2d
	{
	public:
		Colisao2d(Vector4 quadrado, Contexto* ctx);
		Colisao2d(short int raio, Contexto* ctx);
		void defRect(Vector4 quad);
		bool mouseEmCima();
	private:
		Vector4 quadrado;
		Contexto* contexto;
		short int raio = 0;
	};
}
