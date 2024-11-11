
// Copyright (c) 2024 Daniel Oliveira

#include "opcoes.hpp"
#include "src/ui/painel/painel.hpp"
#include <filesystem>

using namespace BubbleUI::Widgets;
using namespace std::filesystem;

Opcoes::Opcoes()
{
}
void Opcoes::atualizar()
{
    const int origem = (int)painel->obterRetangulo().y;
    const int diferenca = painel->posicaoWidget.y - origem;
    Moldura::defCor({ 0.13f, 0.11f, 0.16f, 1.f });
    Moldura::definirPosicao({ painel->widgetPadding.x + painel->posicaoWidget.x, origem + diferenca + painel->widgetPadding.y});
    Moldura::definirTamanho({ static_cast<int>(painel->obterRetangulo().w - painel->widgetPadding.x * 2), static_cast<int>(painel->obterRetangulo().h - diferenca - painel->widgetPadding.y*2)});
    Moldura::atualizar();

    for (auto& botao : botoes)
    {
        botao->atualizar();
    }
    texto.atualizar();
}

void Opcoes::renderizar() const
{
    Moldura::renderizar();

    for (const auto& botao : botoes)
    {
        botao->renderizar();
    }
    texto.renderizar();
}

void Opcoes::recarregar()
{
    diretorios.clear();
    texto.definirPai(this);
    // Itera sobre as pastas de projetos
    for (const auto& pasta : directory_iterator(contexto->dirDoProjeto))
    {
        // Se é uma pasta válida, procura por ícone
        if (!pasta.is_directory())continue;
        // Iterando...
        std::string temp{ "icon.ico" };
        for( const auto& arquivo_projeto : directory_iterator(pasta.path()))
        {
            // Verifica se é um arquivo e se é um ícone
            if (arquivo_projeto.is_regular_file() && arquivo_projeto.path().filename().string() == "icone.png")
            {
                temp = arquivo_projeto.path().string();
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

    texto.definirFonte(13, "assets/fontes/noto_sans/noto_sans.regular.ttf");
    if(botoes.empty())
    texto.definirTexto("Nenhum projeto encontrado");
    else
    texto.definirTexto("" + std::to_string(botoes.size()) + " Projetos encontrados");
}

void Opcoes::adiBotao(const std::string& label,const std::string& icone)
{
    botoes.push_back(std::make_unique<Botao>(path(label).filename().string(), icone, nullptr, true));
    botoes[botoes.size() - 1]->definirPai(this);
}
