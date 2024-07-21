#ifndef LAYOUT_HPP
#define LAYOUT_HPP
#include "src/interface/base/quadrado.hpp"
#include "src/inputs/inputs.h"

class GLFWcursor;
namespace Bubble
{
	namespace Interface
	{
		class BECORE_DLL_API Layout : public Quadrado
		{
		public:
			Layout(Janela j);
			void adicItem(Quadrado& quad) { quad.pai = this; items.push_back(quad); };
			void renderizar() override;
			void atualizar() override;
			void defInputs(Inputs::Inputs* inp) { inputs = inp; };
			void converterMouse(Vector2* mousepos);
		private:
			bool once;
			bool mouseEmCima( Vector2 boxpos, Vector2 boxsize);
			Inputs::Inputs* inputs;
			GLFWcursor* cursorMao;
			Quadrado abajanela;
			std::vector<Quadrado> items;
			std::string nome;
			bool mouseArrastando;
			Vector2 ArrastoPos;
			Vector2 inicioArrastoPos;
			Vector2 inicioMouseArrasto;
		};
	}
}
#endif