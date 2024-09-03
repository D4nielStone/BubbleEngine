#include "colisao2d.hpp"
#include "src/depuracao/debug.hpp"

BubbleUI::Colisao2d::Colisao2d(const Vector4& quadrado, std::shared_ptr<Contexto> ctx) : quadrado(quadrado), contexto(ctx)
{
}

void BubbleUI::Colisao2d::defRect(const Vector4 &quad)
{
	quadrado = quad;
}

bool BubbleUI::Colisao2d::mouseEmCima() const
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