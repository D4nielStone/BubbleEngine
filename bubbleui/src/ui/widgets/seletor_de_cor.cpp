#include "seletor_de_cor.hpp"
#include "src/ui/painel/painel.hpp"

BubbleUI::Widgets::SeletorDeCor::SeletorDeCor(Color* cor, const std::string& label, const Alinhamento& alinhamento)
{
    alinhamentoHorizontal = alinhamento;
    arco_cor = std::make_unique<Imagem>("assets/texturas/icons/arco_cor.png", Vector2{tamanho, tamanho});
    arco_cor->defAlinhamento(alinhamento); arco_cor->padding = true;    arco_cor->quebrarLinha = true;
    frase = label;
    resolucao = 12;
    configurar();
    letra_padding = { 3, 3 };
}

void BubbleUI::Widgets::SeletorDeCor::atualizar()
{
    if (arco_cor)
        arco_cor->atualizar();
    //Texto::atualizar();
}

void BubbleUI::Widgets::SeletorDeCor::renderizar() const
{
    //Texto::renderizar();

    if (arco_cor)
        arco_cor->renderizar();
}

void BubbleUI::Widgets::SeletorDeCor::defPainel(Painel* painel)
{
    Widget::defPainel(painel);
    arco_cor->defPainel(painel);
}
