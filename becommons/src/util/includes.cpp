
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "includes.hpp"

void Material::bind()
{
    if (!shader) shader = new Bubble::Arquivadores::Shader();
    // Ativa o shader
    shader->use();

    // Configura propriedades de material
    shader->setVec3("material.cor_difusa", difusa.r, difusa.g, difusa.b);
    shader->setVec3("material.cor_especular", especular.r, especular.g, especular.b);
    shader->setFloat("material.shininess", shininess);
    shader->setFloat("material.reflexao", reflexao);

    // Gerenciar texturas difusas e especulares
    bool texturaDifusaAtiva = false;
    bool texturaEspecularAtiva = false;

    // Associa todas as texturas disponíveis
    for (GLenum i = 0; i < texturas.size(); i++) {
        glActiveTexture(GL_TEXTURE1 + i);  // Ativa o slot de textura correspondente
        glBindTexture(GL_TEXTURE_2D, texturas[i].ID);  // Vincula a textura ao slot

        // Define o tipo da textura no shader
        shader->setInt(texturas[i].tipo, 1 + i);
        shader->setBool(texturas[i].tipo + "_ativo", true);

        // Verifica se é uma textura difusa ou especular para ativação
        if (texturas[i].tipo == "textura_difusa") {
            texturaDifusaAtiva = true;
        }
        else if (texturas[i].tipo == "textura_especular") {
            texturaEspecularAtiva = true;
        }
    }

    // Ativa ou desativa texturas difusas e especulares conforme o caso
    shader->setBool("textura_difusa_ativo", texturaDifusaAtiva);
    shader->setBool("textura_especular_ativo", texturaEspecularAtiva);

    // Restaura para a unidade de textura padrão
    glActiveTexture(GL_TEXTURE0);
};