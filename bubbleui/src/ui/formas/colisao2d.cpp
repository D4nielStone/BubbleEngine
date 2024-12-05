#include <vector>
#include <memory>
#include <algorithm>
#include "colisao2d.hpp"
#include "src/depuracao/debug.hpp"

namespace BubbleUI {

    Colisao2d::Colisao2d(const Vector4<int>& bounds, std::shared_ptr<Contexto> ctx)
        : bounds(bounds), contexto(ctx)
    {
    }

    void Colisao2d::definirContexto(std::shared_ptr<Contexto> ctx)
    {
        contexto = ctx;
    }

    void Colisao2d::definirBounds(const Vector4<int>& quad)
    {
        bounds = quad;
    }

    Colisao2d::~Colisao2d() {
    }

    bool Colisao2d::mouseEmCima() const {
        const auto posMouse = Vector2(contexto->inputs->mousex, contexto->inputs->mousey);
        return mouseDentro(posMouse);
    }

    bool Colisao2d::mouseDentro(const Vector2& posMouse) const {
        return (posMouse.x >= bounds.x && posMouse.x < (bounds.x + bounds.w) &&
            posMouse.y >= bounds.y && posMouse.y < (bounds.y + bounds.h));
    }
     
} // namespace BubbleUI
