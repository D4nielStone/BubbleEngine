#ifndef TERRENO_H
#define TERRENO_H
#include "src/comum/componente.h"
#include "glad/glad.h"
#include "vector"
#include "includes.h"

namespace Bubble
{
	namespace Componentes
	{
		class Terreno : public Comum::Componente
		{
		private:
			Vertex mVertex; Material mMaterial;
			GLuint VAO, EBO, VBO;
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

#endif // TERRENO_H
