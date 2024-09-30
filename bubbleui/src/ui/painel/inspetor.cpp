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
    BubbleUI::Paineis::msgAdiCam = true;
}
static void adicionarComponenteCodigo()
{
    BubbleUI::Paineis::msgAdiCode = true;
}
static void adicionarComponenteRender()
{
    BubbleUI::Paineis::msgAdiRender = true;
}

BubbleUI::Paineis::Inspetor::Inspetor(std::shared_ptr<Contexto> ctx, std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4& rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    configurar(ctx, rect);
    //popup componentes
    popup_comps = std::make_unique<Util::PopUp>(ctx);
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar camera", adicionarComponenteCamera));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar codigo", adicionarComponenteCodigo));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar renderizador", adicionarComponenteRender));
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
    }
    adicionarWidget(std::make_shared<Widgets::Botao>("Adicionar componente", abrirPopupAdiComp, "assets/texturas/icons/renderizador.png"));
}

void BubbleUI::Paineis::Inspetor::preAtualizacao()
{
    // Recarrega no momento certo
    if (entidade_selecionada != scenemanager->cenaAtual()->entidade_selecionada)
    {
        entidade_selecionada = scenemanager->cenaAtual()->entidade_selecionada;
        recarregar();
    }
    //atualiza popup
    if (popup_comps)
        popup_comps->atualizar();
    // Mostra popup
    if (msgMostrarPopup)
    {
        msgMostrarPopup = false;
        popup_comps->mostrar();
    }
    // Adiciona camera
    if (msgAdiCam) {
        auto componente = std::make_shared<Bubble::Componentes::Camera>();
        componente->configurar();
        entidade_selecionada->adicionarComponente(componente); 
        msgAdiCam = false; recarregar(); }
    // Adiciona codigo
    if (msgAdiCode) {
        auto componente = std::make_shared<Bubble::Componentes::Codigo>("assets/scripts/rotacionar.lua");
        componente->configurar();
        entidade_selecionada->adicionarComponente(componente); 
        msgAdiCode = false; recarregar(); }
    // Adiciona Renderizador
    if (msgAdiRender) {
        auto componente = std::make_shared<Bubble::Componentes::Renderizador>();
        componente->configurar();
        entidade_selecionada->adicionarComponente(componente); 
        msgAdiRender = false; recarregar(); }}

void BubbleUI::Paineis::Inspetor::posRenderizacao() const
{
    //renderiza popup
    if (popup_comps)
        popup_comps->renderizar();
}
