/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "painel.hpp"

namespace BubbleUI {

    // Construtor
    Painel::Painel(const Vector4<int>& rect) 
    {
        retangulo = (rect);
    }
    // Construtor
    Painel::Painel(const char* n, const Vector4<int>& rect) 
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

    std::shared_ptr<Contexto> Painel::obterContexto() const
    {
        return contexto;
    }

    // Ciclo de Vida do Painel: Atualização
    void Painel::atualizar()
    {
        colisao->definirBounds(obterRetangulo());

        // Atualiza a moldura
        arvoreCor = {0.1f, 0.1f, 0.1f};
        if (selecionado)
        {
            if(mostrar_aba && aba)aba->obterCorpo()->defCor(ROXO_CLARO);
        }
        else
        {
            if (mostrar_aba && aba)aba->obterCorpo()->defCor(ROXO_ESCURO);
        }
        if (selecionado && contexto->inputs->mouseButton == GLFW_MOUSE_BUTTON_RIGHT && contexto->inputs->mouseEnter == GLFW_PRESS)
            menuDeContexto->mostrar();
        menuDeContexto->atualizar();

        // Atualiza os widgets
        preAtualizacao();
        if (mostrar_aba && aba)aba->atualizar();
        for (auto& widget : lista_widgets)
        {
            widget->atualizar();
        }
        posAtualizacao();

        Moldura::atualizar();
    }

    void Painel::definirContexto(std::shared_ptr<Contexto> ctx, const char* nome, const Vector4<int>& rect = { 0,0,0,0 })
    {
        Nome = nome;
        configurar(ctx, rect);
    }
    void Painel::definirContexto(std::shared_ptr<Contexto> ctx)
    {
        configurar(ctx);
         
    }
     

    // Ciclo de Vida do Painel: Renderização
    void Painel::renderizar()
    {
        // Passo 1: Desenhar a máscara no stencil buffer
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glDepthMask(GL_FALSE); // Desativar o depth buffer
        glClear(GL_STENCIL_BUFFER_BIT); // Limpar o stencil buffer

        // Desenhar a moldura com bordas arredondadas no stencil buffer
        Moldura::renderizar();

        // Passo 2: Configurar o recorte com stencil
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Permitir a renderização apenas onde o stencil buffer é 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Manter o valor do stencilglDisable(GL_STENCIL_TEST);
        
        preRenderizacao();
        if (mostrar_aba && aba)aba->renderizar();

        // Renderiza os widgets
        for (const auto& widget : lista_widgets)
        {
            widget->renderizar();
        }

        menuDeContexto->renderizar();
        posRenderizacao();
        glDisable(GL_STENCIL_TEST);
    }

    std::vector<std::shared_ptr<BubbleUI::Widget>> Painel::widgets() const
    {
        return lista_widgets;
    }

    void Painel::defCor(const Color& cor)
    {
        Rect::defCor(cor);
    }

    // Configuração do Painel
    void Painel::configurar(std::shared_ptr<Contexto> ctx, const Vector4<int>& rect)
    {
        //retangulo = rect;
        contexto = ctx;
        widgetPadding = { 5, 5 };

        Rect::contexto = ctx;
        
        menuDeContexto= std::make_unique<Util::PopUp>(contexto);

        colisao->definirContexto(contexto);

        Rect::Rect(contexto, retangulo);
        aba= std::make_unique<Aba>(this);
        defCor({ 0.13f, 0.11f, 0.16f, 1.f });
        borda_d = std::make_unique<Rect>(contexto, Vector4<int>{ 0, 0, 0, 0 });
        borda_b = std::make_unique<Rect>(contexto, Vector4<int>{ 0, 0, 0, 0 });
        borda_e = std::make_unique<Rect>(contexto, Vector4<int>{ 0, 0, 0, 0 });
        borda_c = std::make_unique<Rect>(contexto, Vector4<int>{ 0, 0, 0, 0 });
        borda_d->defCor(cor_base);
        borda_b->defCor(cor_base);
        borda_e->defCor(cor_base);
        borda_c->defCor(cor_base);
        ponta_a = std::make_unique<Formas::MeioCirculo>(contexto, Vector4<int>{ 0, 0, espessuraBorda, espessuraBorda });
        ponta_b = std::make_unique<Formas::MeioCirculo>(contexto, Vector4<int>{ 0, 0, -espessuraBorda, espessuraBorda });
        ponta_c = std::make_unique<Formas::MeioCirculo>(contexto, Vector4<int>{ 0, 0, -espessuraBorda, -espessuraBorda });
        ponta_d = std::make_unique<Formas::MeioCirculo>(contexto, Vector4<int>{ 0, 0, espessuraBorda, -espessuraBorda });
        ponta_a->defCor(cor_base);
        ponta_b->defCor(cor_base);
        ponta_c->defCor(cor_base);
        ponta_d->defCor(cor_base);
    }
} // namespace BubbleUI
