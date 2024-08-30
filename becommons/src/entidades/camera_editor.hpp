#pragma once
#include "src/componentes/camera/camera.hpp"
#include "src/componentes/transformacao/transformacao.hpp"
#include "becommons.hpp"
#include "src/inputs/inputs.hpp"

namespace Bubble {
	namespace Entidades {
		class BECOMMONS_DLL_API CameraEditor : public Componentes::Camera
		{
		private:
			glm::vec3 alvoCamera;
			float velocidadeDeMovimento;
			float sensibilidadeDeRotacao;
			float elapsed = 0;
			float yaw, pitch;
			glm::vec3 frente;
		public:
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
			Bubble::Inputs::Inputs* inputs;
			CameraEditor(Bubble::Inputs::Inputs* input);
			~CameraEditor();
			CameraEditor();
			void atualizarDirecao();
			void configurar() override;
			void atualizar() override;
			void olharPara(glm::vec3 pov);
		};
	}
}