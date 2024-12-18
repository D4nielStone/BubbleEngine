/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "separador.hpp"
#include <future>

namespace bubbleui
{
    ancora::ancora(tipoAncoragem tipo, bubbleui::painel* painel) : tipo(tipo), a(nullptr), b(nullptr), divisoria(new separador(tipo, this)), painel(painel), abaAtiva(0) {}

    void ancora::definirContexto(std::shared_ptr<contexto> contexto)
    {
        if (painel) painel->definirContexto(contexto);
        for (auto& p : tabs)
        {
            p->definirContexto(contexto);
        }
        if (a) a->definirContexto(contexto);
        if (b) b->definirContexto(contexto);
        divisoria->contexto = contexto;
        divisoria->inputs = contexto->inputs;
    }

    void ancora::definirPainel(bubbleui::painel* painel) {
        tipo = Nenhum;
        this->painel = painel;
    }

    void ancora::adicionarTab(bubbleui::painel* painel) {
        tipo = Tab;
        tabs.push_back(painel);
    }

    void ancora::atualizarBounds() {
        divisoria->atualizar();
        if (a && b) ///< Se esta dividido então 
        {
            if (tipo == tipoAncoragem::Horizontal) {
                int largura_a = static_cast<int>(bounds.w * divisoria->proporcao());
                a->bounds = bubble::vetor4<int>(bounds.x, bounds.y, largura_a, bounds.h);
                b->bounds = bubble::vetor4<int>(bounds.x + largura_a, bounds.y, bounds.w - largura_a, bounds.h);
            }
            else if (tipo == tipoAncoragem::Vertical) {
                int altura_a = static_cast<int>(bounds.h * divisoria->proporcao());
                a->bounds = bubble::vetor4<int>(bounds.x, bounds.y, bounds.w, altura_a);
                b->bounds = bubble::vetor4<int>(bounds.x, bounds.y + altura_a, bounds.w, bounds.h - altura_a);
            }
            a->atualizarBounds();
            b->atualizarBounds();
        }
        if (painel) painel->definirRetangulo(bounds);
    }

    void ancora::renderizarPaineis() {
        if (painel && tipo == Nenhum) {
            painel->renderizar();
        }

        else if (!tabs.empty() && tipo == Tab) {
            tabs[abaAtiva]->renderizar();  // Renderiza a aba ativa
        }

        // Renderizar recursivamente os nós filhos
        if (a) a->renderizarPaineis();
        if (b) b->renderizarPaineis();
    }


    void ancora::atualizarPaineis() {
        if (painel && tipo == Nenhum) {
            painel->atualizar();
        }
        else if (!tabs.empty() && tipo == Tab) {
            tabs[abaAtiva]->atualizar(); // Renderiza a aba ativa
        }

        // Renderizar recursivamente os nós filhos usando async
        std::future<void> futureA, futureB;

        if (b) {
            b->atualizarPaineis();
        }if (a) {
            a->atualizarPaineis();
        }
    }
}