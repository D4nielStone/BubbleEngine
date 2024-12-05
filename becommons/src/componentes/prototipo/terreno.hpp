

#pragma once
#include "src/componentes/renderizador/renderizador.hpp"
#include "src/util/utils.hpp"
#include "becommons.hpp"
#include "vector"

namespace Bubble
{
	namespace Componentes
	{
		class BECOMMONS_DLL_API Terreno : public Renderizador
		{
		private:
			Vertex mVertex;
			std::vector<std::vector<float>> heightmap;
			float scale;
			int width, depth;
			void calcularNormais();
			void configurarBuffers();
			void desenharModelo() const;
		public:
			Vertex& obterMalha() override;
			Terreno();
			~Terreno();
			void gerarHeightMap();
			void configurar() override;
			void atualizar() override;
		};
	}
}