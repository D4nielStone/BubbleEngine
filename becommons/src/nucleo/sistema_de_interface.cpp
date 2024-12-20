#include <glad/glad.h>
#include "sistema_de_interface.hpp"
#include "src/componentes/renderizador.hpp"
#include "src/componentes/transformacao.hpp"
#include "src/componentes/camera.hpp"
#include "src/nucleo/cena.hpp"
#include "src/arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <src/componentes/texto.hpp>
#include <src/arquivadores/fonte.hpp>

bubble::shader* shader_texto{ nullptr };

namespace bubble
{
    void sistemaInterface::atualizar(double deltaTime)
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        reg->cada<bubble::texto>([&](const uint32_t ent) 
            {
                auto comp_text = reg->obter<bubble::texto>(ent);
                texto(*shader_texto, comp_text->frase, -1, -1, 0.005f, {1, 1, 1, 1});
            }
        );
    }

    void sistemaInterface::inicializar(bubble::cena* cena)
    {
        this->cena = cena;
        this->reg = cena->obterRegistro();

        if (!shader_texto) shader_texto = new bubble::shader("texto.vert", "texto.frag");

        glGenVertexArrays(1, &text_VAO);
        glGenBuffers(1, &text_VBO);
        glBindVertexArray(text_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void bubble::sistemaInterface::texto(shader& s, const std::string& texto, float x, float y, float escala, bubble::cor color)
    {
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

        // activate corresponding render state	
        s.use();
        s.setCor("textColor", color);
        s.setMat4("projection", glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_VAO);

        // iterate through all characters
        std::string::const_iterator c;
        auto& chs = bubble::gerenciadorFontes::obterInstancia().obter("noto_sans.regular.ttf");
        for(char32_t ca : texto)
        {
            if (chs.empty())
                return;
            bubble::caractere ch = chs.at(ca);
            
            float xpos = x + ch.apoio.x * escala;
            float ypos = y - (ch.tamanho.y - ch.apoio.y) * escala;

            float w = ch.tamanho.x * escala;
            float h = ch.tamanho.y * escala;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.id);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.avanco >> 6) * escala; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}