/* Projetos

	Uma janela para a visualização, edição( deletar, mudar nome etc.) e seleção
	de projetos da engine.
*/

/* Inclusões */
//#include "becore/src/nucleo/projeto.hpp"
#include "becommons/src/arquivadores/imageloader.hpp"
#include "bubbleui/src/ui/contexto/contexto.hpp"
#include "bubbleui/src/ui/painel/visualizador_de_projetos.hpp"
#include "bubbleui/src/ui/util/barra_menu.hpp"

/* Declarações globais */
GLFWwindow* janela{ nullptr };
std::shared_ptr<BubbleUI::Contexto> contexto_ui{ nullptr };
std::shared_ptr<BubbleUI::Paineis::VisualizadorDeProjetos> painel{ nullptr };
std::unique_ptr<BubbleUI::Util::BarraMenu> barra_menu{ nullptr };
constexpr const char* title = "Bubble Engine - Project Manager - (C) 2024 Daniel Oliveira";