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

namespace BubbleUI
{
	enum TipoAncoragem
	{
		Nenhum,
		Vertical,
		Horizontal,
		Tab
	};
	struct Ancora
	{
		/** 
		 *  @brief  construtor
		 *	@param tipo Alinhamendo da ancora
		 *	@param painel painel caso None
		 */
		Ancora(TipoAncoragem tipo, Painel* painel = nullptr);
		TipoAncoragem tipo;
		Ancora* a, * b;	///< Paineis de vivisão ( caso dividido )
		std::vector<Painel*> tabs; ///< Tabs para caso seja do tipo tab
		Painel* painel; ///< Painel filho
		unsigned int abaAtiva;
		Vector4<int> bounds{30, 30, 600, 400};	   ///< limites do dock
		void definirContexto(std::shared_ptr<Contexto>);
		void definirPainel(Painel* painel);
		void adicionarTab(Painel* painel);
		void atualizarBounds();
		void atualizarPaineis();
		void renderizarPaineis();
	};
}