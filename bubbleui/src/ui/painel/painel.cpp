#include "painel.hpp"

namespace BubbleUI {

    // Construtor
    Painel::Painel(std::shared_ptr<Contexto> ctx, const Vector4& rect)
    {
        configurar(ctx, rect);
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
        arvoreCor = {0.1f, 0.1f, 0.1f};
        if (selecionado)
        {
            bordaCima->atualizar(); bordaBaixo->atualizar();
            bordaEsq->atualizar(); bordaDir->atualizar();
            corrigirLimite();
            if(mostrar_aba)aba->obterCorpo()->defCor({ 0.4f, 0.0f, 0.4f, 1 });
        }
        else
        {
            if (mostrar_aba)aba->obterCorpo()->defCor({ 0.1f, 0.1f, 0.1f, 1 });
        }
        if (selecionado && contexto->inputs->mouseButton == GLFW_MOUSE_BUTTON_RIGHT && contexto->inputs->mouseEnter == GLFW_PRESS)
            menuDeContexto->mostrar();
        menuDeContexto->atualizar();

        // Atualiza os widgets
        preAtualizacao();
        if (mostrar_aba)aba->atualizar(); else { posicaoWidget = { 0, 0 }; };
        for (auto& widget : lista_widgets)
        {
            widget->atualizar();
        }
        posAtualizacao();

        arrastando = false;
        // Atualiza a moldura
        Moldura::atualizar();
    }

    // Ciclo de Vida do Painel: Renderização
    void Painel::renderizar() const
    {
        glScissor(retangulo.x -1, (contexto->tamanho.height - (static_cast<int>(retangulo.y) + 1) - retangulo.h), retangulo.w + 2, retangulo.h + 2);

        Moldura::renderizar();
        preRenderizacao();
        if (mostrar_aba)aba->renderizar();

        // Renderiza os widgets
        for (const auto& widget : lista_widgets)
        {
            widget->renderizar();
        }

        menuDeContexto->renderizar();
        posRenderizacao();
    }

    // Configuração do Painel
    void Painel::configurar(std::shared_ptr<Contexto> ctx, const Vector4& rect)
    {
        contexto = ctx;
        widgetPadding = { 5, 5 };
        tamanhoMinimo = { 100, 15 };
        retangulo = rect;

        // Inicializa bordas e moldura
        bordaCima = std::make_unique<Separador>(CIMA, this);
        bordaBaixo = std::make_unique<Separador>(BAIXO, this);
        bordaEsq = std::make_unique<Separador>(ESQUERDA, this);
        bordaDir = std::make_unique<Separador>(DIREITA, this);


        menuDeContexto= std::make_unique<Util::PopUp>(contexto);
        aba= std::make_unique<Aba>(this);

        this->contexto = ctx;
        this->retangulo = retangulo;
        Rect::Rect(ctx, retangulo);
        defCor({ 0.13f, 0.11f, 0.16f, 1.f });
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
        if (obterRetangulo().h < tamanhoMinimo.y && redimensionamentoAtual== BAIXO)
        {
            definirTamanho({ obterRetangulo().w, tamanhoMinimo.y });
        }
        if (obterRetangulo().w < tamanhoMinimo.x && redimensionamentoAtual== ESQUERDA)
        {
            adiPos({ retangulo.w - tamanhoMinimo.x, 0 });
            definirTamanho({ tamanhoMinimo.x, obterRetangulo().h });
        }
        if (obterRetangulo().h < tamanhoMinimo.y && redimensionamentoAtual== CIMA)
        {
            adiPos({ obterRetangulo().h - tamanhoMinimo.y, 0 });
            definirTamanho({ obterRetangulo().w, tamanhoMinimo.y });
        }
    }

    // Cursor Normal
    bool Painel::cursorNormal() const
    {
        return !(bordaEsq->cursor() || bordaBaixo->cursor() || bordaCima->cursor() || bordaDir->cursor());
    }
} // namespace BubbleUI