#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include <cmath>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

namespace Bubble {
    namespace Util {
        class PerlinNoise {
        public:
            PerlinNoise(unsigned int seed = std::default_random_engine::default_seed);

            double noise(double x, double y) const;

        private:
            std::vector<int> p;

            // Função fade para suavizar a interpolação
            static double fade(double t);

            // Função lerp para interpolação linear
            static double lerp(double t, double a, double b);

            // Função gradiente
            static double grad(int hash, double x, double y);
        };
    }
}
#endif