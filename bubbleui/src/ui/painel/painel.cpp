// Copyright (c) 2024 Daniel Oliveira

#include "painel.hpp"

namespace BubbleUI {

    // Construtor
    Painel::Painel(const Vector4& rect) 
    {
        retangulo = (rect);
    }
    // Construtor
    Painel::Painel(const char* n, const Vector4& rect) 
    {
        retangulo = (rect);
        Nome = n;
    }


    // Widgets
    void Painel::adicionarWidget(std::shared_ptr<Widget> widget)
    {
        widget->definirPai(this);
        lista_widgets.push_back(widget);
    }

    Vector2 Painel::obterTamanhoMinimo() const
    {
        return tamanhoMinimo;
    }

    std::shared_ptr<Contexto> Painel::obterContexto() const
    {
        return contexto;
    }

    // Ciclo de Vida do Painel: Atualização
    void Painel::atualizar()
    {
        Moldura::atualizar();
        // Atualiza a moldura
        arvoreCor = {0.1f, 0.1f, 0.1f};
        if (selecionado)
        {
            bordaCima->atualizar(); bordaBaixo->atualizar();
            bordaEsq->atualizar(); bordaDir->atualizar();
            if(mostrar_aba)aba->obterCorpo()->defCor(ROXO_CLARO);
        }
        else
        {
            if (mostrar_aba)aba->obterCorpo()->defCor(ROXO_ESCURO);
        }
        corrigirLimite();
        Rect::atualizar();
        if (selecionado && contexto->inputs->mouseButton == GLFW_MOUSE_BUTTON_RIGHT && contexto->inputs->mouseEnter == GLFW_PRESS)
            menuDeContexto->mostrar();
        menuDeContexto->atualizar();

        // Atualiza os widgets
        preAtualizacao();
        if (mostrar_aba)aba->atualizar();
        for (auto& widget : lista_widgets)
        {
            widget->atualizar();
        }
        posAtualizacao();

        arrastando = false;

        corpo.definirRetangulo(obterRetangulo());
        corpo.atualizar();
    }

    void Painel::definirContexto(std::shared_ptr<Contexto> ctx, const char* nome, const Vector4& rect = { 0,0,0,0 })
    {
        Nome = nome;
        configurar(ctx, rect);
    }
    void Painel::definirContexto(std::shared_ptr<Contexto> ctx)
    {
        configurar(ctx);
    }

    // Ciclo de Vida do Painel: Renderização
    void Painel::renderizar() const
    {

        // Passo 1: Desenhar a máscara no stencil buffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Desativar a escrita de cores
        glDepthMask(GL_FALSE); // Desativar o depth buffer
        glClear(GL_STENCIL_BUFFER_BIT); // Limpar o stencil buffer

        // Desenhar a moldura com bordas arredondadas no stencil buffer
        Moldura::renderizar();

        // Passo 2: Configurar o recorte com stencil
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Reativar a escrita de cores
        glDepthMask(GL_TRUE); // Reativar o depth buffer
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Permitir a renderização apenas onde o stencil buffer é 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Manter o valor do stencil

        // Desenhar o conteúdo dentro da área recortada
        corpo.renderizar();

        preRenderizacao();
        if (mostrar_aba && aba)aba->renderizar();

        // Renderiza os widgets
        for (const auto& widget : lista_widgets)
        {
            widget->renderizar();
        }

        menuDeContexto->renderizar();
        posRenderizacao();
    }

    std::vector<std::shared_ptr<BubbleUI::Widget>> Painel::widgets() const
    {
        return lista_widgets;
    }

    void Painel::defCor(const Color& cor)
    {
        Rect::defCor(cor);
        corpo.defCor(cor);
    }

    // Configuração do Painel
    void Painel::configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect)
    {
        retangulo = rect;
        contexto = ctx;
        widgetPadding = { 5, 5 };
        tamanhoMinimo = { 100, 15 };

        Rect::contexto = ctx;
        // Inicializa bordas e moldura
        bordaCima = std::make_unique<Separador>(CIMA, this);
        bordaBaixo = std::make_unique<Separador>(BAIXO, this);
        bordaEsq = std::make_unique<Separador>(ESQUERDA, this);
        bordaDir = std::make_unique<Separador>(DIREITA, this);


        menuDeContexto= std::make_unique<Util::PopUp>(contexto);
        aba= std::make_unique<Aba>(this);

        this->retangulo = retangulo;
        Rect::Rect(ctx, retangulo);
        corpo.definirBuffers(ctx, retangulo);
        defCor({ 0.13f, 0.11f, 0.16f, 1.f });
        corpo.defCor({ 0.13f, 0.11f, 0.16f, 1.f });
        borda_d = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
        borda_b = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
        borda_e = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
        borda_c = std::make_unique<Rect>(contexto, Vector4{ 0, 0, 0, 0 });
        borda_d->defCor(cor_base);
        borda_b->defCor(cor_base);
        borda_e->defCor(cor_base);
        borda_c->defCor(cor_base);
        ponta_a = std::make_unique<Formas::MeioCirculo>(contexto, Vector4{ 0, 0, espessuraBorda, espessuraBorda });
        ponta_b = std::make_unique<Formas::MeioCirculo>(contexto, Vector4{ 0, 0, -espessuraBorda, espessuraBorda });
        ponta_c = std::make_unique<Formas::MeioCirculo>(contexto, Vector4{ 0, 0, -espessuraBorda, -espessuraBorda });
        ponta_d = std::make_unique<Formas::MeioCirculo>(contexto, Vector4{ 0, 0, espessuraBorda, -espessuraBorda });
        ponta_a->defCor(cor_base);
        ponta_b->defCor(cor_base);
        ponta_c->defCor(cor_base);
        ponta_d->defCor(cor_base);
    }

    // Controle de Limites e Redimensionamento
    void Painel::corrigirLimite()
    {
        if (obterRetangulo().w < tamanhoMinimo.x && redimensionamentoAtual == DIREITA)
        {
            definirTamanho({ tamanhoMinimo.x, obterRetangulo().h });
        }
        if (obterRetangulo().h < tamanhoMinimo.y && redimensionamentoAtual == BAIXO)
        {
            definirTamanho({ tamanhoMinimo.x, obterRetangulo().w });
        }
    }

    // Cursor Normal
    bool Painel::cursorNormal() const
    {
        return !(bordaEsq->cursor() || bordaBaixo->cursor() || bordaCima->cursor() || bordaDir->cursor());
    }
} // namespace BubbleUI
