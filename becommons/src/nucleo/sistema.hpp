/** @copyright Copyright (c) 2024 Daniel Oliveira */

/**
 * @file sistema.hpp
 * @brief Gerencia os sistemas no loop principal
 */

#pragma once
#include "src/entidades/entidade.hpp"

 /**
  * @class Sistema
  * @brief Classe base para sistemas do loop principal
  */

namespace bubble
{
    class sistema {
    public:
        /**
         * @brief Construtor virtual da classe Sistema
         * @param nome Nome do sistema
         */
        sistema(bubble::registro* reg) : reg(reg) {}
        sistema() = default;

        /**
         * @brief Destrutor virtual para garantir a destruição adequada das subclasses
         */
        virtual ~sistema() = default;

        /**
         * @brief Inicializa o sistema
         */
        virtual void inicializar() = 0;

        /**
         * @brief Atualiza o sistema dentro do loop principal
         * @param deltaTime Tempo desde a última atualização (em segundos)
         */
        virtual void atualizar(double deltaTime) = 0;

    protected:
        registro* reg;
    };
}