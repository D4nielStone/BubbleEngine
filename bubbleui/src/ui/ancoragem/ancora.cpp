/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "separador.hpp"

using namespace BubbleUI;

Ancora::Ancora(TipoAncoragem tipo, Painel* painel) : tipo(tipo), a(nullptr), b(nullptr), divisoria(new Separador(tipo, this)), painel(painel), abaAtiva(0) {}

void Ancora::definirContexto(std::shared_ptr<Contexto> contexto)
{ 
	if (painel) painel->definirContexto(contexto);
	for (auto& p : tabs)
	{
		p->definirContexto(contexto);
	}
	if(a) a->definirContexto(contexto);
	if(b) b->definirContexto(contexto);
    divisoria->contexto = contexto;
    divisoria->inputs = contexto->inputs;
}

void Ancora::definirPainel(Painel* painel) {
    tipo = Nenhum;
	this->painel = painel;
}

void Ancora::adicionarTab(Painel* painel) {
    tipo = Tab;
	tabs.push_back(painel);
}

void Ancora::atualizarBounds() {
    divisoria->atualizar();
    if (a && b) ///< Se esta dividido então 
    {
        if (tipo == TipoAncoragem::Horizontal) {
            int largura_a = static_cast<int>(bounds.w * divisoria->proporcao());
            a->bounds = Vector4<int>(bounds.x, bounds.y, largura_a, bounds.h);
            b->bounds = Vector4<int>(bounds.x + largura_a, bounds.y, bounds.w - largura_a, bounds.h);
        }
        else if (tipo == TipoAncoragem::Vertical) {
            int altura_a = static_cast<int>(bounds.h * divisoria->proporcao());
            a->bounds = Vector4<int>(bounds.x, bounds.y, bounds.w, altura_a);
            b->bounds = Vector4<int>(bounds.x, bounds.y + altura_a, bounds.w, bounds.h - altura_a);
        }
        a->atualizarBounds();
        b->atualizarBounds();
    }
    if (painel) painel->definirRetangulo(bounds);
}

void Ancora::renderizarPaineis() {
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

#include <future>

void Ancora::atualizarPaineis() {
    if (painel && tipo == Nenhum) {
        painel->atualizar();
    }
    else if (!tabs.empty() && tipo == Tab) {
        tabs[abaAtiva]->atualizar(); // Renderiza a aba ativa
    }

    // Renderizar recursivamente os nós filhos usando async
    std::future<void> futureA, futureB;

    if (a) {
        futureA = std::async(std::launch::async, &Ancora::atualizarPaineis, a); // Executa em paralelo
    }
    if (b) {
        futureB = std::async(std::launch::async, &Ancora::atualizarPaineis, b); // Executa em paralelo
    }

    // Espera os resultados das tarefas assíncronas
    if (futureA.valid()) {
        futureA.get(); // Aguarda a execução de 'a'
    }
    if (futureB.valid()) {
        futureB.get(); // Aguarda a execução de 'b'
    }
}
