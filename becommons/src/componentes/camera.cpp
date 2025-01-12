#include "camera.hpp"
#include "transformacao.hpp"
#include <src/nucleo/fase.hpp>
#include <os/janela.hpp>
bubble::camera::camera( const bool orth)
	: flag_orth(orth)
{
}

glm::mat4 bubble::camera::obtViewMatrix()
{
	if (!transform)
		transform = fase_atual->obterRegistro()->obter<transformacao>(meu_objeto);

	// A posição da câmera é definida pela transformação associada.
	posicao = transform->posicao;

	// O vetor "para cima" (up) da câmera.
	glm::vec3 up = transform->cima;

	// Alvo da câmera: ou é um ponto específico ou um ponto calculado com base na rotação.
	glm::vec3 alvo;
	if (transform->alvo)
	{
		alvo = *transform->alvo; // Alvo explícito definido.
	}
	else
	{
		// Calcula o alvo baseado na posição e rotação da câmera.
		glm::vec3 frente = glm::vec3(
			cos(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x)),
			sin(glm::radians(transform->rotacao.x)),
			sin(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x))
		);
		alvo = posicao + glm::normalize(frente); // Ponto para onde a câmera está "olhando".
	}

	// Usa glm::lookAt para calcular a matriz de visão.
	return glm::lookAt(posicao, alvo, up);
}

glm::mat4 bubble::camera::obtProjectionMatrix()
{
	if (flag_orth && viewport_ptr)
	{
		float largura = viewport_ptr->w;
		float altura = viewport_ptr->h;
		aspecto = largura / altura;

		return glm::ortho(-escala * aspecto, escala * aspecto, -escala, escala, corte_curto, corte_longo);

	}
	else if (viewport_ptr)
	{
		float largura = viewport_ptr->w;
		float altura = viewport_ptr->h;
		aspecto = largura / altura;

		return glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
	}
}

inline vet3 bubble::camera::telaParaNDC(vet2 screenPoint, vet2 screenSize)
{
	// screenPoint: Coordenadas de tela (x, y), geralmente em pixels.
	// screenSize: Tamanho da tela (largura, altura), em pixels.
	// Retorna: Coordenadas normalizadas no dispositivo (NDC).

	float xNDC = (2.0f * screenPoint.x) / screenSize.x - 1.0f;
	float yNDC = 1.0f - (2.0f * screenPoint.y) / screenSize.y;
	float zNDC = 1.0f; // Geralmente definido como 1 para a profundidade inicial (plano de projeção)

	return vet3(xNDC, yNDC, zNDC);
}

bubble::raio bubble::camera::pontoParaRaio(vet2 screenPoint) {
	// Converte screenPoint (x, y) para Normalized Device Coordinates
	vet3 ndc = telaParaNDC(screenPoint, vet2(viewport_ptr->w, viewport_ptr->h));

	// Converte para o espaço de câmera
	glm::vec4 cameraSpace = glm::inverse(obtProjectionMatrix()) * glm::vec4(ndc.x, ndc.y, ndc.z, 1.f);

	// Converte para o espaço do mundo
	glm::vec4 worldSpaceDirection = glm::inverse(obtViewMatrix()) * cameraSpace;

	// Cria o raio a partir da posição da câmera e da direção
	raio ray{};
	ray.origem = posicao; // Origem do raio é a posição da câmera
	ray.direcao = normalize(worldSpaceDirection); // Direção normalizada

	return ray;
}