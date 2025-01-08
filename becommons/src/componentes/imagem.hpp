/** @copyright Copyright (c) 2024 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include <string>
#include <src/util/vetor2.hpp>
#include "src/arquivadores/imageloader.hpp"
#include <filesystem>

namespace bubble
{
	struct imagem : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_IMAGEM;
		const std::string arquivo	{ "" };
		vetor2<int> limite			{ 20, 20 };
		vetor2<int> padding			{ 0,0 };
		bool flip					{ false };
		unsigned int id;
		imagem(const std::string& diretorio) : arquivo(diretorio) 
		{
			std::string dir{};
			if (std::filesystem::exists(diretorio))
				dir = (diretorio);
			else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
			{

				dir = (std::filesystem::absolute(diretorio).string().c_str());
			}
			id = bubble::textureLoader::obterInstancia().carregarTextura(dir, &limite.x, &limite.y);
		}
	};
} 