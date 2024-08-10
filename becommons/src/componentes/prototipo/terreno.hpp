#pragma once
#include "src/comum/componente.hpp"
#include "src/util/includes.hpp"
#include "becommons.hpp"
#include "vector"

namespace Bubble
{
	namespace Componentes
	{
		class BECOMMONS_DLL_API Terreno : public Comum::Componente
		{
		private:
			Vertex mVertex; Material mMaterial;
			std::vector<std::vector<float>> heightmap;
			float scale;
			int width, depth;
			void calcularNormais();
			void configurarBuffers();
			void desenharModelo();
			void atualizarMaterial();
		public:
			Terreno();
			~Terreno();
			void gerarHeightMap();
			void configurar() override;
			void atualizar(float deltaTime) override;
		};
	}
}