#include "becommons.hpp"
#include "entidade.hpp"
#include "rapidjson/stringbuffer.h"
#include "src/cena/scenemanager.hpp"

using namespace Bubble::Entidades;
Entidade::~Entidade() {}
Entidade::Entidade(const char* name) : ativado(true), Nome(name), transformacao(std::make_shared<Bubble::Componentes::Transformacao>())
		{
			adicionarComponente(transformacao);
		}
Entidade::Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto)
			: transformacao(std::make_shared<Bubble::Componentes::Transformacao>()) { // Use std::make_shared aqui
			adicionarComponente(transformacao);
			carregarModelo(std::move(arquivo_objeto));
		}
Entidade::Entidade() : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()) {
			adicionarComponente(std::make_shared<Bubble::Componentes::Transformacao>());
		};
void Entidade::atualizar(float deltaTime) {
			for (auto& c : Componentes) 
			{
				if(ativado && !dynamic_cast<Componentes::Camera*>(c.get()) && !dynamic_cast<Componentes::Renderizador*>(c.get()))
				c->atualizar(deltaTime);
			}
		}
void Entidade::renderizar()
		{
			for (auto& c : Componentes)
			{
				if (ativado && dynamic_cast<Componentes::Renderizador*>(c.get()))
					obterTransformacao()->atualizar();
					c->atualizar();
			}
		}
std::string *Entidade::nome() {
			return &Nome;
		}
void Entidade::carregarModelo(Bubble::Arquivadores::Arquivo3d object_file) {
			auto renderizador = std::make_shared<Bubble::Componentes::Renderizador>(object_file);
			adicionarComponente(renderizador);
		}
std::shared_ptr<Bubble::Comum::Componente> Entidade::obterComponente(const std::string& nome) {
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					return c;
				}
			}
			return nullptr;
		}
std::vector<std::shared_ptr<Bubble::Comum::Componente>> Entidade::obterComponentes(const std::string& nome) {
			std::vector<std::shared_ptr<Bubble::Comum::Componente>> comps;
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					comps.push_back(c);
				}
			}
			return comps;
		}
std::shared_ptr<Bubble::Componentes::Transformacao> Entidade::obterTransformacao() {
			return transformacao;
		}
const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& Entidade::listaDeComponentes() const {
			return Componentes;
		}
void Entidade::adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente) {
			componente->definirPai(this);
			Componentes.push_back(componente);
		}
rapidjson::Value Entidade::serializar(rapidjson::Document* v)
		{
			rapidjson::Value obj(rapidjson::kObjectType);
			obj.AddMember("nome", rapidjson::Value().SetString(Nome.c_str(), v->GetAllocator()), v->GetAllocator());
			rapidjson::Value array(rapidjson::kArrayType);

			array.PushBack(transformacao->serializar(v), v->GetAllocator());
			
			obj.AddMember("transformacao", array, v->GetAllocator());
			return obj;
		}
bool Entidade::parse(rapidjson::Value& entidade)
		{
			Nome = entidade["nome"].GetString();
			Debug::emitir("ENTIDADE", "  " + Nome + ":");
			return true;
		}