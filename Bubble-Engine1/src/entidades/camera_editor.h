#pragma once
#include "src/componentes/camera/camera.h"
#include "src/componentes/transformacao/transformacao.h"

namespace Bubble {
	namespace Entidades {
		class CameraEditor : public Componentes::Camera
		{
		public:
			CameraEditor();
			CameraEditor(float fov, float aspecto, float znear, float zfar);
			void configurar() override;
			void atualizar(float deltaTime) override;
			std::shared_ptr<Bubble::Componentes::Transformacao> transformacao;
		};
	}
}