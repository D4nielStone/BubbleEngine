/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "inspetor.hpp"
#include "src/ui/widgets/caixa_de_texto.hpp"
#include "src/ui/widgets/arvore.hpp"
#include "src/ui/widgets/checkbox.hpp"
#include "src/ui/widgets/botao.hpp"
#include "src/ui/widgets/seletor_de_cor.hpp"
#include "src/ui/items/item_botao.hpp"
#include <src/componentes/prototipo/terreno.hpp>
using namespace BubbleUI::Paineis;
using namespace BubbleUI;

static void abrirPopupAdiComp()
{
    msgMostrarPopup = true;
}

Inspetor::Inspetor(std::shared_ptr<Bubble::Cena::SceneManager> scenemanager, const Vector4<int>& rect)
    : scenemanager(scenemanager), nome_atual(new std::string(""))
{
    Nome = "Inspetor";
    retangulo = rect;
    
}

Inspetor::~Inspetor() = default;

void BubbleUI::Paineis::Inspetor::definirContexto(std::shared_ptr<Contexto> ctx)
{
    Painel::definirContexto(ctx);
    //popup componentes
    popup_comps = std::make_unique<Util::PopUp>(ctx);
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar camera", &msgAdiCam));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar codigo", &msgAdiCode));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar renderizador", &msgAdiRender));
    popup_comps->adiItem(std::make_shared<Items::Botao>("adicionar terreno", &msgAdiTerr));
    // adiciona chackbox para ativar/desativar entidade
    if (entidade_selecionada) {
        auto ent_atv_chk = std::make_shared<Widgets::CheckBox>(&entidade_selecionada->ativado);
        adicionarWidget(ent_atv_chk);
    ent_atv_chk->quebrarLinha = false;
    }
    // Verifica se o contexto e o scenemanager são válidos antes de usar
    if (scenemanager && ctx)    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, ""));
    else                        Debug::emitir(Erro, "Scenemanager ou contexto inválido");// Log de erro
}

void Inspetor::recarregar()
{
    if (!scenemanager) return;

    lista_widgets.clear();
    // Verifica entidade selecionada e muda nome da entidade atual
    if (entidade_selecionada) nome_atual = entidade_selecionada->nomeptr();

    // adiciona chackbox para ativar/desativar entidade
    auto ent_atv_chk = std::make_shared<Widgets::CheckBox>(&entidade_selecionada->ativado);
    adicionarWidget(ent_atv_chk);
    ent_atv_chk->quebrarLinha = false;
    // adiciona caixa de texto para o nome da entidade
    adicionarWidget(std::make_shared<Widgets::CaixaTexto>(nome_atual, "Nome da entidade"));
    // adiciona uma arvore para cada componente da entidade
    if(entidade_selecionada)
    for (const auto& componente : entidade_selecionada->listaDeComponentes())
    {
        const auto name = std::string("assets/texturas/icons/" + std::string(componente->nome()) + ".png");
        auto arvore = std::make_shared<Widgets::Arvore>(componente->nome(), nullptr, name);
        adicionarWidget(arvore);
        
        desvendarWidgets(componente->variaveis, arvore);
    }
    adicionarWidget(std::make_shared<Widgets::Botao>("Adicionar componente", abrirPopupAdiComp, "assets/texturas/icons/renderizador.png"));
}

void Inspetor::preAtualizacao()
{
    /*if (!scenemanager) return;
    // Recarrega no momento certo
    if (scenemanager->cenaAtual() && entidade_selecionada != scenemanager->cenaAtual()->entidade_selecionada)
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
        BubbleUI::tarefa([this]() {
            /*auto componente = std::make_shared<Bubble::Componentes::Camera>();
            (entidade_selecionada != nullptr) && entidade_selecionada->adicionarComponente(componente);
            msgAdiCam = false; recarregar();
            });
    }
    // Adiciona codigo
    if (msgAdiCode) {
        BubbleUI::tarefa([this]() {
            auto componente = std::make_shared<Bubble::Componentes::Codigo>("assets/scripts/rotacionar.lua");
            (entidade_selecionada != nullptr) && entidade_selecionada->adicionarComponente(componente);
            msgAdiCode = false; recarregar();
            });
    }
    // Adiciona Renderizador
    if (msgAdiRender) {
        BubbleUI::tarefa([this]() {
            auto componente = std::make_shared<Bubble::Componentes::Renderizador>();
            (entidade_selecionada != nullptr) && entidade_selecionada->adicionarComponente(componente);
            msgAdiRender = false; recarregar();    });
    }
    // Adiciona Terreno
    if (msgAdiTerr) {
        BubbleUI::tarefa([this]() {
            auto componente = std::make_shared<Bubble::Componentes::Terreno>();
            (entidade_selecionada != nullptr) && entidade_selecionada->adicionarComponente(componente);
            msgAdiTerr = false; recarregar();
            });
    }*/
}

void Inspetor::posRenderizacao() const
{
    //renderiza popup
    if (popup_comps)
        popup_comps->renderizar();
}

void Inspetor::desvendarWidgets(const std::vector<std::any>& variaveis, std::shared_ptr<Widgets::Arvore> arvore)
{
    // Verifica se possui arvore
    for (const auto& variavel : variaveis)
    {
        if (variavel.type() == typeid(ArvoreID))
        {
            auto newVar = variavel._Cast<ArvoreID>();
            auto arvore_recursiva = std::make_shared<Widgets::Arvore>(newVar->second, nullptr);
            arvore->adiFilho(arvore_recursiva);
            desvendarWidgets(newVar->first, arvore_recursiva);
        }else
    // Verifica se possui check box
        if (variavel.type() == typeid(CheckBoxID))
        {
            auto newVar = variavel._Cast<CheckBoxID>();
            arvore->adiFilho(std::make_shared<Widgets::CheckBox>(newVar->first, newVar->second, Alinhamento::Direita));
        }else
    // Verifica se possui caixa de texto
        if (variavel.type() == typeid(CaixaDeTextoID))
        {
            auto newVar = variavel._Cast<CaixaDeTextoID>();
            arvore->adiFilho(std::make_shared<Widgets::CaixaTexto>(newVar->first, newVar->second));
        }else
    // Verifica se possui vetor3
        if (variavel.type() == typeid(Vector3ID))
        {
            auto newVar = variavel._Cast<Vector3ID>();

            auto arvorev3 = std::make_shared<Widgets::Arvore>(newVar->second, nullptr);
            arvore->adiFilho(arvorev3);
            arvorev3->adiFilho(std::make_shared<Widgets::CaixaTexto>(&newVar->first->x, "x"));
            arvorev3->adiFilho(std::make_shared<Widgets::CaixaTexto>(&newVar->first->y, "y"));
            arvorev3->adiFilho(std::make_shared<Widgets::CaixaTexto>(&newVar->first->z, "z"));
        }else
    // Verifica se possui seletor de cor
        if (variavel.type() == typeid(SeletorDeCorID))
        {
            auto newVar = variavel._Cast<SeletorDeCorID>();
            arvore->adiFilho(std::make_shared<Widgets::SeletorDeCor>(newVar->first, newVar->second));
        }
    }
}
