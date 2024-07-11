#pragma once
#include "src/componentes/camera/camera.h"
#include "src/componentes/transformacao/transformacao.h"
#include "becore.h"

namespace Bubble {
	namespace Entidades {
		class BECORE_DLL_API CameraEditor : public Componentes::Camera
		{
		private:
			glm::vec3 alvoCamera;
		public:
			CameraEditor();
			~CameraEditor();
			CameraEditor(float fov, float aspecto, float znear, float zfar);
			void configurar() override;
			void atualizar(float deltaTime) override;
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
			void olharPara(glm::vec3 pov);
		};
	}
}