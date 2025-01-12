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

	// A posi��o da c�mera � definida pela transforma��o associada.
	posicao = transform->posicao;

	// O vetor "para cima" (up) da c�mera.
	glm::vec3 up = transform->cima;

	// Alvo da c�mera: ou � um ponto espec�fico ou um ponto calculado com base na rota��o.
	glm::vec3 alvo;
	if (transform->alvo)
	{
		alvo = *transform->alvo; // Alvo expl�cito definido.
	}
	else
	{
		// Calcula o alvo baseado na posi��o e rota��o da c�mera.
		glm::vec3 frente = glm::vec3(
			cos(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x)),
			sin(glm::radians(transform->rotacao.x)),
			sin(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x))
		);
		alvo = posicao + glm::normalize(frente); // Ponto para onde a c�mera est� "olhando".
	}

	// Usa glm::lookAt para calcular a matriz de vis�o.
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
	float zNDC = 1.0f; // Geralmente definido como 1 para a profundidade inicial (plano de proje��o)

	return vet3(xNDC, yNDC, zNDC);
}

bubble::raio bubble::camera::pontoParaRaio(vet2 screenPoint) {
	// Converte screenPoint (x, y) para Normalized Device Coordinates
	vet3 ndc = telaParaNDC(screenPoint, vet2(viewport_ptr->w, viewport_ptr->h));

	// Converte para o espa�o de c�mera
	glm::vec4 cameraSpace = glm::inverse(obtProjectionMatrix()) * glm::vec4(ndc.x, ndc.y, ndc.z, 1.f);

	// Converte para o espa�o do mundo
	glm::vec4 worldSpaceDirection = glm::inverse(obtViewMatrix()) * cameraSpace;

	// Cria o raio a partir da posi��o da c�mera e da dire��o
	raio ray{};
	ray.origem = posicao; // Origem do raio � a posi��o da c�mera
	ray.direcao = normalize(worldSpaceDirection); // Dire��o normalizada

	return ray;
}