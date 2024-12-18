/** @copyright Copyright (c) 2024 Daniel Oliveira */
#include "arquivos.hpp"
#include "src/ui/painel/painel.hpp"
#include <filesystem>

using namespace BubbleUI::Widgets;
using namespace std::filesystem;

Arquivos::Arquivos()
{
}
void Arquivos::atualizar()
{
    const int origem = (int)painel->obterRetangulo().y;
    const int diferenca = painel->posicaoWidget.y - origem;
    Moldura::definirPosicao({ painel->widgetPadding.x + painel->posicaoWidget.x, origem + diferenca + painel->widgetPadding.y});
    Moldura::definirTamanho({ static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x * 2), static_cast<int>(painel->obterRetangulo().h - diferenca - painel->widgetPadding.y*2)});
    Moldura::atualizar();

    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
}

void Arquivos::renderizar()
{
    // Passo 1: Desenhar a máscara no stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glbubble::corMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Desativar a escrita de cores
    glDepthMask(GL_FALSE); // Desativar o depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Limpar o stencil buffer

    // Desenhar a moldura com bordas arredondadas no stencil buffer
    Moldura::renderizar();

    // Passo 2: Configurar o recorte com stencil
    glbubble::corMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Reativar a escrita de cores
    glDepthMask(GL_TRUE); // Reativar o depth buffer
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Permitir a renderização apenas onde o stencil buffer é 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Manter o valor do stencil

    // Desenhar o conteúdo dentro da área recortada
    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
}

void Arquivos::recarregar(const char* dir, const char* tipo_arquivo)
{
    Moldura::defCor(ROXO_ESCURO);
    if (!is_directory(dir))
        return;

    botoes.clear();
    // Itera sobre as pastas de projetos
    for (const auto& pasta : directory_iterator(dir))
    {
        // Se é uma pasta válida, adicionar ícone folder
        std::string temp{ "folder.png" };
        if (!pasta.is_directory() && pasta.is_regular_file())
        {
            //se não é uma pasta, verifica estensão do arquivo
            const auto extensao = pasta.path().extension().string();
            if (tipo_arquivo != "*" && extensao != tipo_arquivo)return;
            if (extensao == ".icon" || extensao == ".png" || extensao == ".jpg")
            {
                temp = pasta.path().string();
            }
            if (extensao == ".beproj")
            {
                temp = "icon.ico";
            }
        }
        diretorios.push_back(std::pair(pasta.path().string(), temp));
    }
    //
    // Adiciona botão para cada projeto
    for (const auto& diretorio : diretorios)
    {
        adiBotao(diretorio.first, diretorio.second);
    }
    diretorios.clear();
}

void Arquivos::adiBotao(const std::string& label,const std::string& icone)
{
    botoes.emplace_back(std::make_unique<Botao>(path(label).filename().string(), icone, nullptr, false));
    botoes[botoes.size() - 1]->definirPai(this);
    botoes[botoes.size() - 1]->quebrarLinha = true;
}