#pragma once
#include "src/ui/contexto/contexto.hpp"
#include "src/util/vetor4.hpp
src/util/vetor2.hpp"

namespace BubbleUI
{
	class Colisao2d : public std::enable_shared_from_this<Colisao2d>
	{
	public:
		Colisao2d() = default;
		Colisao2d(const bubble::vetor4<int> &quadrado, std::shared_ptr<Contexto> ctx);
        ~Colisao2d();
        void definirContexto(std::shared_ptr<Contexto> ctx);
		void definirBounds(const bubble::vetor4<int> &quad);
		bool mouseEmCima() const;
        bool mouseDentro(const Vector2& posMouse) const;
	private:
		bubble::vetor4<int> bounds{};
		std::shared_ptr<Contexto> contexto;
		short int raio = 0;
	};
}
