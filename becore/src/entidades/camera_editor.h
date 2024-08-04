#pragma once
#include "src/componentes/camera/camera.h"
#include "src/componentes/transformacao/transformacao.h"
#include "becore.h"
#include "src/inputs/inputs.h"

namespace Bubble {
	namespace Entidades {
		class BECORE_DLL_API CameraEditor : public Componentes::Camera
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
			void atualizar(float deltaTime = 1) override;
			void olharPara(glm::vec3 pov);
		};
	}
}