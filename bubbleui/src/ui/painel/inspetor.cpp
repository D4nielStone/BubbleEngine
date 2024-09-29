#include "inspetor.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/arvore.hpp"
#include "src/ui/widgets/checkbox.hpp"
#include "src/ui/widgets/botao.hpp"
#include "src/ui/items/item_botao.hpp"

static void abrirPopupAdiComp()
{
    BubbleUI::Paineis::msgMostrarPopup = true;
}
static void adicionarComponenteCamera()
{

}

BubbleUI::Paineis::Inspetor::Inspetor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    configurar(ctx, rect);
    //popup componentes
    popup_comps = std::make_unique<Util::PopUp>(ctx);
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar camera", adicionarComponenteCamera));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar codigo", adicionarComponenteCamera));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar renderizador", adicionarComponenteCamera));
    // Verifica se o contexto e o scenemanager são válidos antes de usar
    adicionarWidget(std::make_shared<Widgets::CheckBox>(nullptr));
    if (scenemanager && ctx)    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Debug::Erro, "Scenemanager ou contexto inválido");// Log de erro
}

BubbleUI::Paineis::Inspetor::~Inspetor() = default;

void BubbleUI::Paineis::Inspetor::recarregar()
{
    lista_widgets.clear();
    // Verifica entidade selecionada e muda nome da entidade atual
    if (entidade_selecionada) nome_atual = entidade_selecionada->nomeptr();
    // Verifica se o contexto e o scenemanager são válidos antes de usar
    adicionarWidget(std::make_shared<Widgets::CheckBox>(&entidade_selecionada->ativado));
    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, "Nome da entidade"));
    for (const auto& componente : entidade_selecionada->listaDeComponentes())
    {
        const auto name = std::string("assets/texturas/icons/" + std::string(componente->nome()) + ".png");
        auto arvore = std::make_shared<Widgets::Arvore>(componente->nome(), nullptr, name);
        adicionarWidget(arvore);

        if (dynamic_cast<Bubble::Componentes::Renderizador*>(componente.get()))
        {
            auto renderer = static_cast<Bubble::Componentes::Renderizador*>(componente.get());
            auto check_box = std::make_shared<Widgets::CheckBox>(&renderer->visualizarWireFrame, "Visualizar como wireframe");
            check_box->quebrarLinha = true;
            arvore->adiFilho(check_box);
        }
    }
    adicionarWidget(std::make_shared<Widgets::Botao>("Adicionar componente", abrirPopupAdiComp, "assets/texturas/icons/renderizador.png"));
}

void BubbleUI::Paineis::Inspetor::preAtualizacao()
{
    if (msgMostrarPopup)
    {
        msgMostrarPopup = false;
        popup_comps->defPos({100, 100});
        popup_comps->defTam({100, 100});
        popup_comps->mostrar();
    }
    // Recarrega no momento certo
    if (entidade_selecionada != scenemanager->cenaAtual()->entidade_selecionada)
    {
        entidade_selecionada = scenemanager->cenaAtual()->entidade_selecionada;
        recarregar();
    }
    //atualiza popup
    if (popup_comps)
        popup_comps->atualizar();
}

void BubbleUI::Paineis::Inspetor::posRenderizacao() const
{
    //renderiza popup
    if (popup_comps)
        popup_comps->renderizar();
}
