#include "perlinnoise.h"

namespace Bubble
{
	namespace Util
	{
		PerlinNoise::PerlinNoise(unsigned int seed)
        {
            // Gerar um vetor de permutação com valores de 0 a 255
            p.resize(256);
            std::iota(p.begin(), p.end(), 0);

            // Embaralhar o vetor usando o seed fornecido
            std::default_random_engine engine(seed);
            std::shuffle(p.begin(), p.end(), engine);

            // Duplicar o vetor de permutação para evitar overflow
            p.insert(p.end(), p.begin(), p.end());
        }
        double PerlinNoise::noise(double x, double y) const {
            // Encontrar a posição da célula da grade
            int X = static_cast<int>(std::floor(x)) & 255;
            int Y = static_cast<int>(std::floor(y)) & 255;

            // Encontre a posição relativa no cube unitário
            x -= std::floor(x);
            y -= std::floor(y);

            // Calcular suavização usando a função fade
            double u = fade(x);
            double v = fade(y);

            // Obter os gradientes
            int aa, ab, ba, bb;
            aa = p[p[X] + Y];
            ab = p[p[X] + Y + 1];
            ba = p[p[X + 1] + Y];
            bb = p[p[X + 1] + Y + 1];

            // Misturar os resultados dos gradientes e interpolar
            double result = lerp(v, lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
                lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));

            return (result + 1.0) / 2.0;  // Ajustar o resultado para o intervalo [0, 1]
        }
        double PerlinNoise::fade(double t)
        {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }
        double PerlinNoise::lerp(double t, double a, double b)
        {
            return a + t * (b - a);
        }
        double PerlinNoise::grad(int hash, double x, double y)
        {
            int h = hash & 7;  // Permutação de 8
            double u = h < 4 ? x : y;
            double v = h < 4 ? y : x;
            return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
        }
	}
}