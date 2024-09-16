#include "painel.hpp"

namespace BubbleUI {

    // Construtor
    Painel::Painel(std::shared_ptr<Contexto> ctx, const Vector4& rect)
    {
        configurar(ctx, rect);
    }

    // Manipulação de Tamanho e Posição
    void Painel::definirTamanho(const Vector2& tam)
    {
        retangulo.w = tam.x;
        retangulo.h = tam.y;
    }

    void Painel::definirPosicao(const Vector2& pos)
    {
        retangulo.x = pos.x;
        retangulo.y = pos.y;
    }

    void Painel::adicionarTamanho(const Vector2& tam)
    {
        retangulo.w += tam.x;
        retangulo.h += tam.y;
    }

    void Painel::adicionarPosicao(const Vector2& pos)
    {
        retangulo.x += pos.x;
        retangulo.y += pos.y;
    }

    // Widgets
    void Painel::adicionarWidget(std::shared_ptr<Widget> widget)
    {
        widget->defPainel(this);
        lista_widgets.push_back(widget);
    }

    // Obtenção de Dados
    Vector4 Painel::obterRetangulo() const
    {
        return retangulo;
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
        if (selecionado)
        {
            bordaCima->atualizar(); bordaBaixo->atualizar();
            bordaEsq->atualizar(); bordaDir->atualizar();
            corrigirLimite();
            menuDeContexto->atualizar();
            aba->obterCorpo()->defCor({ 0.4f, 0.0f, 0.4f, 1 });
        }
        else
        {
            menuDeContexto->esconder();
            aba->obterCorpo()->defCor({ 0.1f, 0.1f, 0.1f, 1 });
        }

        // Atualiza a moldura
        moldura.defPos({ static_cast<int>(retangulo.x), static_cast<int>(retangulo.y) });
        moldura.defTam({ retangulo.w, retangulo.h });
        moldura.atualizar();

        // Controle de Pop-ups
        if (mostrarPopup)
        {
            menuDeContexto->mostrar();
            mostrarPopup= false;
        }
        else if (esconderPopup)
        {
            menuDeContexto->esconder();
            esconderPopup= false;
        }

        // Atualiza os widgets
        preAtualizacao();
        aba->atualizar();
        for (auto& widget : lista_widgets)
        {
            widget->atualizar();
        }
        posAtualizacao();

        arrastando = false;
    }

    // Ciclo de Vida do Painel: Renderização
    void Painel::renderizar() const
    {
<<<<<<< HEAD
        glScissor(retangulo.x -1, (contexto->tamanho.height - (static_cast<int>(retangulo.y) - 1) - retangulo.h), retangulo.w + 2, retangulo.h + 2);
=======
        glScissor(retangulo.x -1, (contexto->tamanho.height - (static_cast<int>(retangulo.y) + 1) - retangulo.h), retangulo.w + 2, retangulo.h + 2);
>>>>>>> master

        moldura.renderizar();
        preRenderizacao();
        aba->renderizar();

        // Renderiza os widgets
        for (const auto& widget : lista_widgets)
        {
            widget->renderizar();
        }

        menuDeContexto->renderizar();
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

        moldura = Formas::Moldura(contexto);
        moldura.defCor({ 0.1f, 0.1f, 0.1f });

        menuDeContexto= std::make_unique<Util::PopUp>(contexto);
        aba= std::make_unique<Aba>(this);
    }

    // Controle de Limites e Redimensionamento
    void Painel::corrigirLimite()
    {
        if (retangulo.w < tamanhoMinimo.x && redimensionamentoAtual== DIREITA)
        {
            retangulo.w = tamanhoMinimo.x;
        }
        if (retangulo.h < tamanhoMinimo.y && redimensionamentoAtual== BAIXO)
        {
            retangulo.h = tamanhoMinimo.y;
        }
        if (retangulo.w < tamanhoMinimo.x && redimensionamentoAtual== ESQUERDA)
        {
            retangulo.x += retangulo.w - tamanhoMinimo.x;
            retangulo.w = tamanhoMinimo.x;
        }
        if (retangulo.h < tamanhoMinimo.y && redimensionamentoAtual== CIMA)
        {
            retangulo.y += retangulo.h - tamanhoMinimo.y;
            retangulo.h = tamanhoMinimo.y;
        }
    }

    // Cursor Normal
    bool Painel::cursorNormal() const
    {
        return !(bordaEsq->cursor() || bordaBaixo->cursor() || bordaCima->cursor() || bordaDir->cursor());
    }
} // namespace BubbleUI