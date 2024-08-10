#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include <cmath>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include "becommons.hpp"

namespace Bubble {
    namespace Util {
        class BECOMMONS_DLL_API PerlinNoise {
        public:
            PerlinNoise(unsigned int seed = std::default_random_engine::default_seed);

            double noise(double x, double y) const;

        private:
            std::vector<int> p;

            // Fun��o fade para suavizar a interpola��o
            static double fade(double t);

            // Fun��o lerp para interpola��o linear
            static double lerp(double t, double a, double b);

            // Fun��o gradiente
            static double grad(int hash, double x, double y);
        };
    }
}
#endif