/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "seletor_de_cor.hpp"
#include "src/ui/painel/painel.hpp"
#include <algorithm>

constexpr auto PI = 3.14159265359;
using namespace BubbleUI::Widgets;

static void HSVtoRGB(float h, float s, float v, float* r, float* g, float* b) {
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
    case 0: *r = v, *g = t, *b = p; break;
    case 1: *r = q, *g = v, *b = p; break;
    case 2: *r = p, *g = v, *b = t; break;
    case 3: *r = p, *g = q, *b = v; break;
    case 4: *r = t, *g = p, *b = v; break;
    case 5: *r = v, *g = p, *b = q; break;
    }
}

SeletorDeCor::SeletorDeCor(Color* cor, const std::string& label, const Alinhamento& alinhamento)
: cor_callback(cor)
{
    alinhamentoHorizontal = Alinhamento::Esquerda;
    arco_cor = std::make_unique<Imagem>("assets/texturas/icons/arco_cor.png", Vector2{static_cast<int>(raio*2), static_cast<int>(raio * 2) });
    arco_cor->defAlinhamento(alinhamento); arco_cor->padding = true;
    arco_cor->quebrarLinha = true;
    arco_cor->flip = true;
    frase = label;
    quebrarLinha = true;
    resolucao = 12;
    definirFonte();
    letra_padding = { 3, 3 };
}

void SeletorDeCor::atualizar()
{
    if (arco_cor)
        arco_cor->atualizar();
    calcularCor();
    Texto::atualizar();
}

void SeletorDeCor::renderizar()
{
    if (arco_cor)
        arco_cor->renderizar();
    Texto::renderizar();
}

void SeletorDeCor::definirPai(Formas::Moldura* painel)
{
    Widget::definirPai(painel);
    arco_cor->definirPai(painel);
}

void SeletorDeCor::calcularCor()
{
    float cx = arco_cor->obtRect().x + arco_cor->obtRect().w/2.f;
    float cy = arco_cor->obtRect().y + arco_cor->obtRect().h /2.f;
    float dx = inputs->mousex - cx;
    float dy = inputs->mousey - cy;

    angulo = atan2(dy, dx);  // Radians
    hue = angulo * (180.0 / PI);
    if (hue < 0) {
        hue += 360;
    }
    // Cálculo da saturação
    float distancia = sqrt(dx * dx + dy * dy);
    saturacao = std::min(distancia / raio, 1.0f); // Garante que a saturação esteja entre 0 e 1

    // converte hsv para rgb
    HSVtoRGB((hue)/360, saturacao, brilho, &cor_callback->r,&cor_callback->g,&cor_callback->b);
}
