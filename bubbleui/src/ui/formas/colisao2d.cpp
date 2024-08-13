#include "colisao2d.hpp"
#include "src/depuracao/debug.hpp"

BubbleUI::Colisao2d::Colisao2d(Vector4 quad, Contexto* ctx) : quadrado(quad), contexto(ctx)
{
}

BubbleUI::Colisao2d::Colisao2d(short int r, Contexto* ctx) : raio(r), contexto(ctx)
{
}

void BubbleUI::Colisao2d::defRect(Vector4 quad)
{
	quadrado = quad;
}

bool BubbleUI::Colisao2d::mouseEmCima()
{
	if (   contexto->inputs->mousex > quadrado.x 
		&& contexto->inputs->mousex < (quadrado.x + quadrado.w)
		&& contexto->inputs->mousey > quadrado.y 
		&& contexto->inputs->mousey < (quadrado.y + quadrado.h)
	   )
		return true;
	else
		return false;
}
