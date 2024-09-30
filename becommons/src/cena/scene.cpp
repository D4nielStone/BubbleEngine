#include "Scene.hpp"
#include "glad/glad.h"

namespace Bubble::Cena
{
    // Uma cena é criada
    // \param name: para o nome da cena
    Scene::Scene(const std::string &name) : Name(std::make_shared<std::string>(name)), skybox(std::make_unique<Util::Skybox>()) {
        //Debug::emitir("CENA", std::string(name) + " criada");
    }
    Scene::~Scene() {}
    // Deve adicionar entidade
    void Scene::carregarComponentes(std::shared_ptr<Entidades::Entidade> entidade) {
        for (auto& componente : entidade->listaDeComponentes()) {
            if (!componente->carregado())
                componente->configurar();
            if (componente->nome() == "Renderizador")
            {
                // adiciona o renderizador ao nome correspondente do material
                entidadesParaRenderizar[static_cast<Componentes::Renderizador*>(componente.get())->obterMalha().material.nome].first.push_back(entidade);
                entidadesParaRenderizar[static_cast<Componentes::Renderizador*>(componente.get())->obterMalha().material.nome].second = static_cast<Componentes::Renderizador*>(componente.get())->obterMalha().material;
            }
        }
        for (auto& filho : entidade->obterFilhos()) {
            carregarComponentes(filho);
        }
    }

    void Scene::criarEntidade(const std::string &path, const char* nome_entidade)
    {
        Arquivadores::Arquivo3d arquivo_3d(path); arquivo_3d.carregar();
        adicionarEntidade(std::make_shared<Entidades::Entidade>(arquivo_3d));
    }

    void Scene::adicionarEntidade(std::shared_ptr<Entidades::Entidade> gameObject) {
        if (!existeEntidade(gameObject.get())) {
            carregarComponentes(gameObject);  // Carrega os componentes da entidade e dos filhos
            Entidades.push_back(std::move(gameObject));  // Adiciona objeto à lista de entidades
        }
    }
    bool carregou_arquivo3d{ false };
    // Deve renderizar Cena
    void Scene::renderizar(const InputMode modo) const {
        
        if (modo == Editor) {
            camera_editor.renderizar();
            skybox->renderizar(camera_editor.obterProjMatrixMat(), camera_editor.obterViewMatrixMat());
        }
        else if(camera_principal->meuObjeto->ativado) {
            camera_principal->renderizar();
            skybox->renderizar(camera_principal->obterProjMatrixMat(), camera_principal->obterViewMatrixMat());
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return;
        }
        for (auto& pair : entidadesParaRenderizar)
        {
            Componentes::atualizarMaterial(pair.second.second, shader);
            for (auto& entidade : pair.second.first)
            {
                entidade->renderizar();
            }
        }
    }
    // Deve atualizar Cena
    void Scene::atualizar(float aspectoDoEditor, float aspectoDoJogo) {
        // Verifica se a tarefa já terminou sem bloquear e se não foi carregado ainda
        //if (!carregou_arquivo3d && tarefaCriarEntidade.first.valid() && tarefaCriarEntidade.first.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        //    adicionarEntidade(std::make_shared<Entidades::Entidade>(tarefaCriarEntidade.second));
        //    carregou_arquivo3d = true; // Marca como carregado
        //}
        camera_editor.atualizarAspecto(aspectoDoEditor);
        camera_editor.atualizar();
        if (camera_principal) {
            camera_principal->atualizarAspecto(aspectoDoJogo);
            camera_principal->atualizar();
        }
        bool existe_obj_selecionado{ false };
        for (auto& obj : Entidades) 
        {
            // Verifica seleção e ativação do obj
            if (obj->selecionada && !existe_obj_selecionado) { entidade_selecionada = obj; existe_obj_selecionado = true; }
            // Ignora se não ativo
            if (!obj->ativado || !obj->ativado_root)
                continue;
            obj->atualizar();
            // defini camera atual
            if ((!camera_principal || obj->selecionada)&& obj->obterComponente("Camera")) {
                camera_principal = static_cast<Componentes::Camera*>(obj->obterComponente("Camera").get());
            }
            atualizarFilhos(obj);
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

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        for(auto& entidade : Entidades)
        for (auto& c : entidade->listaDeComponentes()) 
        {
            c->configurar();
        }
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
            Debug::emitir("CENA", *Name + std::string(":"));
        }
        else
        {
            Debug::emitir(Debug::Erro, "O membro 'nome' não foi encontrado na cena");
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
                    Debug::emitir(Debug::Erro, "parse de entidades");
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
            Debug::emitir(Debug::Erro, "parse de entidades");
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