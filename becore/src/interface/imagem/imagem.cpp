#include "imagem.h"
#include "glad/glad.h"
#include "src/depuracao/debug.h"
#include "src/arquivadores/imageloader.h"

Bubble::Interface::Imagem::Imagem(std::string path, float escala)
{
	// Carregar os dados da imagem
	Arquivadores::ImageLoader imageLoader(path);
	unsigned char* data = imageLoader.obterDados();
	int width = imageLoader.getWidth();  // Suponha que você tenha métodos para obter a largura e a altura
	int height = imageLoader.getHeight();

	// Gerar e vincular a textura
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// Definir o formato e os dados da textura
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tamanho *= (escala / 100.f);
	// Configurar parâmetros de filtragem e envolvimento da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Desvincular a textura
	glBindTexture(GL_TEXTURE_2D, 0);
}
Bubble::Interface::Imagem::Imagem(unsigned int id) : ID(id)
{
	tamanho *= (escala / 100.f);
}
void Bubble::Interface::Imagem::renderizar()
{
	shader.use();
	glBindTexture(GL_TEXTURE_2D, ID);
	shader.setInt("imagem", 1);
	shader.setInt("textura", 0);
	shader.setVec2("quadrado.tamanho", tamanho.x , tamanho.y  );
	shader.setVec2("quadrado.posicao", posicaoG.x, posicaoG.y );
	shader.setVec3("cor", cor.r, cor.g, cor.b);
	shader.setMat4("projecao", glm::value_ptr(projecao));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Bubble::Interface::Imagem::atualizar()
{
	Quadrado::atualizar();
}
