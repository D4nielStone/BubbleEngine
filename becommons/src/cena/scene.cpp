/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "Scene.hpp"
#include "src/entidades/entidade.hpp"
#include "glad/glad.h"
#include "src/componentes/prototipo/terreno.hpp"

namespace Bubble::Cena
{
    void PipeLine::adicionarAoMapa(std::shared_ptr<Entidades::Entidade> entidade, Comum::Componente* componente, const std::string& tipo) {
        auto& malha = static_cast<Componentes::Renderizador*>(componente)->obterMalha();
        entidadesParaRenderizar[malha.material.ID].first.push_back(entidade);
        entidadesParaRenderizar[malha.material.ID].second = &malha.material;
        Debug::emitir("PipeLine", "Adicionando entidade com " + tipo);
    }

    void PipeLine::adicionarEntidade(std::shared_ptr<Entidades::Entidade> entidade) {
        for (auto& componente : entidade->listaDeComponentes()) {
            if (!componente->carregado())
                componente->configurar();

            const auto& nome = componente->nome();
            if (nome == "Renderizador" || nome == "Terreno") {
                adicionarAoMapa(entidade, componente.get(), nome);
            }
        }
        for (auto& filho : entidade->obterFilhos()) {
            adicionarEntidade(filho);
        }
    }

    void PipeLine::removerEntidade(std::shared_ptr<Entidades::Entidade> ent) {
        // Itera sobre os componentes da entidade
        for (auto& componente : ent->listaDeComponentes()) {
            if (componente->nome() == "Renderizador") {
                auto& materialID = static_cast<Componentes::Renderizador*>(componente.get())->obterMalha().material.ID;
                auto& listaEntidades = entidadesParaRenderizar[materialID].first;

                // Remove a entidade da lista associada ao material
                listaEntidades.erase(std::remove(listaEntidades.begin(), listaEntidades.end(), ent), listaEntidades.end());

                // Remove o material do mapa se não houver mais entidades associadas
                if (listaEntidades.empty()) {
                    entidadesParaRenderizar.erase(materialID);
                }
            }
            else if (componente->nome() == "Terreno") {
                auto& materialID = static_cast<Componentes::Terreno*>(componente.get())->obterMalha().material.ID;
                auto& listaEntidades = entidadesParaRenderizar[materialID].first;

                // Remove a entidade da lista associada ao material
                listaEntidades.erase(std::remove(listaEntidades.begin(), listaEntidades.end(), ent), listaEntidades.end());

                // Remove o material do mapa se não houver mais entidades associadas
                if (listaEntidades.empty()) {
                    entidadesParaRenderizar.erase(materialID);
                }
            }
        }

        // Remover os filhos recursivamente
        for (auto& filho : ent->obterFilhos()) {
            removerEntidade(filho);
        }
    }


    void PipeLine::renderizar(Componentes::Camera* cam)
    {
        glDisable(GL_DEPTH_TEST);
        cam->renderizar();
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        // Itera sobre todos os pares de material e entidades
        for (auto& [materialID, entidadesMaterial] : entidadesParaRenderizar) {
            // Pega o ponteiro do material atual
            Material* material = entidadesMaterial.second;

            // Configura o material (shaders, texturas, etc.)
            if (material) {
                material->bind();
            }

            cam->atualizar();
            cam->atualizarShader();

            // Itera pelas entidades associadas ao material atual
            for (auto& entidade : entidadesMaterial.first) {
                // Renderiza a entidade
                entidade->renderizar();
            }
        }

        //Desligar framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // Uma cena é criada
    // \param name: para o nome da cena
    Scene::Scene(const std::string& name) : Name(std::make_shared<std::string>(name)) {
        Debug::emitir("Cena", std::string(name) + " criada");
    }
    Scene::~Scene() {}
    // Deve adicionar entidade

    std::shared_ptr<Entidades::Entidade> Scene::criarEntidade(const std::string& path, const char* nome_entidade)
    {
        Arquivadores::Arquivo3d arquivo_3d(path);
        adicionarEntidade(std::make_shared<Entidades::Entidade>(arquivo_3d));
        return Entidades[Entidades.size() - 1];
    }

    void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (!existeEntidade(gameObject.get())) {
            renderizadores.adicionarEntidade(gameObject);
            Entidades.push_back(std::move(gameObject));
        }
    }

    void Scene::removerEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (gameObject != nullptr && existeEntidade(gameObject.get())) {
            renderizadores.removerEntidade(gameObject);  // Carrega os componentes da entidade e dos filhos
            auto it = std::find(Entidades.begin(), Entidades.end(), gameObject);
            if (it != Entidades.end())
                Entidades.erase(it);  // Adiciona objeto à lista de entidades
            else if (gameObject->pai)
                gameObject->pai->removerFilho(gameObject);
            else {
                Debug::emitir("Cena", "Não foi possivel remover entidade");
                return;
            }
            Debug::emitir("Cena", "entidade removida");
            gameObject->liberar();
            gameObject.reset();
            return;
        }
    }
    bool carregou_arquivo3d{ false };
    // Deve renderizar Cena
    void Scene::renderizar(const InputMode modo) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (modo == Editor) {
            renderizadores.renderizar(&camera_editor);
        }
        else {
            renderizadores.renderizar(camera_principal);
        }
    }

    // Deve atualizar Cena
    void Scene::atualizar(float aspectoDoEditor, float aspectoDoJogo) {
        
        bool existe_obj_selecionado{ false };
        for (const auto& obj : Entidades) 
        {
            // Ignora se não ativo
            if (!obj->ativado || !obj->ativado_root)
                continue;
            // Verifica seleção e ativação do obj
            if (obj->selecionada && !existe_obj_selecionado) { entidade_selecionada = obj; existe_obj_selecionado = true; }
            obj->atualizar();
            // defini camera atual
            auto cam = obj->obterComponente("Camera");
            if ((!camera_principal || obj->selecionada)&& cam) {
                camera_principal = static_cast<Componentes::Camera*>(cam.get());
            }
            atualizarFilhos(obj);
        }
        camera_editor.atualizarAspecto(aspectoDoEditor);
        camera_editor.atualizar();
        if (camera_principal) {
            camera_principal->atualizarAspecto(aspectoDoJogo);
            camera_principal->atualizar();
        }
    }

    void Scene::atualizarFilhos(std::shared_ptr<Entidades::Entidade> entidade) {
        for (auto& filho : entidade->obterFilhos()) {
            if (filho->selecionada)entidade_selecionada = filho;
            filho->atualizar();
            if (filho->obterComponente("Camera")) {
                camera_principal = static_cast<Componentes::Camera*>(filho->obterComponente("Camera").get());
            }
            atualizarFilhos(filho);
        }
    }

    // Deve carregar Cena
    void Scene::carregar() {
        camera_editor.configurar();

        glCullFace(GL_BACK);

        for (auto& entidade : Entidades)
            renderizadores.adicionarEntidade(entidade);
    }
    // Deve serializar ela mesma ( isso é, passar para o documento json seus dados )
    void Scene::serializar(rapidjson::Document* doc) const {
        doc->SetObject();
        doc->AddMember("nome", rapidjson::Value().SetString(Name->c_str(), doc->GetAllocator()), doc->GetAllocator());

        rapidjson::Value arr(rapidjson::kArrayType);
        for (auto& entidade : Entidades) {
            arr.PushBack(entidade->serializar(doc), doc->GetAllocator());
        }

        doc->AddMember("entidades", arr, doc->GetAllocator());
    }
    // Deve ler o arquivo json e obter os dados
    bool Scene::parse(rapidjson::Document& document)
    {
        // Deve verificar se o documento tem o membro "nome" e se é uma string
        if (document.HasMember("nome") && document["nome"].IsString())
        {
            *Name = document["nome"].GetString();
            Debug::emitir("Cena", *Name + std::string(":"));
        }
        else
        {
            Debug::emitir(Erro, "O membro 'nome' não foi encontrado na cena");
            return false;
        }
        // Deve analisar entidades
        if (document.HasMember("entidades") && document["entidades"].IsArray())
        {
            rapidjson::Value& entidades = document["entidades"];
            for (rapidjson::SizeType i = 0; i < entidades.Size(); ++i)
            {
                std::shared_ptr<Entidades::Entidade> entidadetmp = std::make_shared<Entidades::Entidade>();
                if (!entidadetmp->parse(entidades[i]))
                {
                    Debug::emitir(Erro, "parse de entidades");
                    return false;
                }
                else
                {
                    adicionarEntidade(entidadetmp);
                }
            }
        }
        else
        {
            Debug::emitir(Erro, "parse de entidades");
            return false;
        }
        return true;
    }
    // Deve retornar se a entidade existe na lista de entidades
    bool Scene::existeEntidade(Entidades::Entidade* entidade) const {
        for (const auto& obj : Entidades) {
            if (entidadeExisteRecursivo(obj, entidade)) {
                return true;
            }
        }
        return false;
    }

    bool Scene::entidadeExisteRecursivo(std::shared_ptr<Entidades::Entidade> obj, Entidades::Entidade* entidade) const {
        if (obj.get() == entidade) {
            return true;
        }
        for (const auto& filho : obj->obterFilhos()) {
            if (entidadeExisteRecursivo(filho, entidade)) {
                return true;
            }
        }
        return false;
    }

    // Deve retornar ponteiro do nome
    std::shared_ptr<std::string> Scene::nomeptr()
    {
        return Name;
    }
    // Deve retornar nome
    std::string Scene::nome() const {
        return *Name;
    }
}
