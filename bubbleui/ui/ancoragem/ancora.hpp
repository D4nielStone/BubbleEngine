/** @copyright Copyright (c) 2024 Daniel Oliveira */
/**
 * @file ancora.hpp
 * @brief A ancora alinha os paineis baseado em um tipo de alinhamento, podendo ser vertical ou orizontal
 *       
 *	 Ancora (Nó folha)
 *	└── Painel (Conteúdo final)
 *
 *		+---------------------+----------------------+
 *      |       Painel A      |        Painel B      |
 *      |                     +----------------------+
 *      |                     |        Painel C      |
 *      +---------------------+----------------------+
 *       
 *		+---------------------+
 *		|       Painel A      |  <- a
 *		+---------------------+
 *		|       Painel B      |  <- b
 *		+---------------------+
*/
#include "src/ui/painel/painel.hpp"

namespace bubbleui
{
	struct separador;	///< Definicao antecipada da classe Spearador
	enum tipoAncoragem
	{
		Nenhum,
		Vertical,
		Horizontal,
		Tab
	};
	struct ancora
	{
		/** 
		 *  @brief  construtor
		 *	@param tipo Alinhamendo da ancora
		 *	@param painel painel caso None
		 */
		ancora(tipoAncoragem tipo, painel* painel = nullptr);
		tipoAncoragem tipo;
		ancora* a, * b;	///< Paineis de vivisão ( caso dividido )
		std::vector<painel*> tabs; ///< Tabs para caso seja do tipo tab
		painel* painel; ///< Painel filho
		separador* divisoria;
		unsigned int abaAtiva;
		bubble::vetor4<int> bounds{30, 30, 600, 400};	   ///< limites do dock
		void definirContexto(std::shared_ptr<contexto>);
		void definirPainel(bubbleui::painel* painel);
		void adicionarTab(bubbleui::painel* painel);
		void atualizarBounds();
		void atualizarPaineis();
		void renderizarPaineis();
	};
}