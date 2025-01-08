#include "fase.hpp"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <src/componentes/codigo.hpp>
#include <src/componentes/texto.hpp>
#include <src/componentes/transformacao.hpp>
#include <src/componentes/propriedades.hpp>
#include <src/componentes/renderizador.hpp>
#include <src/componentes/imagem.hpp>
#include <iostream>
#include <os/janela.hpp>

using namespace rapidjson;
using namespace bubble;

fase::fase() : _Mnome("")
{
	srender.inicializar(this);
	sinterface.inicializar(this);
	fase_atual = this;
}

fase::fase(const char* diretorio) : diretorio(diretorio)
{
	srender.inicializar(this);
	sinterface.inicializar(this);

	/// efetua a analise do json
	fase_atual = this;

	if(std::filesystem::exists(diretorio))
		analizar(diretorio);
	else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
	{
		
		analizar(std::filesystem::absolute(diretorio).string().c_str());
	}
}

static void analizarMalha(bubble::malha* m, rapidjson::Value& malha)
{
	/// cor difusa

	if (malha.HasMember("cor_difusa"))
		m->material.difusa =
	{
		malha["cor_difusa"].GetArray()[0].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[1].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[2].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[3].GetFloat() / 255,
	};
	/// recebe luz
	if (malha.HasMember("recebe_luz"))
		m->material.recebe_luz = malha["recebe_luz"].GetBool();
	/// itera instancias
	if (malha.HasMember("instancias"))
	{
		for (auto& instancia : malha["instancias"].GetArray())
		{
			m->instancias_pos.push_back( glm::vec3(instancia[0].GetFloat(),instancia[1].GetFloat(),instancia[2].GetFloat()));
		}
	}
}
static void analizarEntidades(Document& doc, fase* f)
{
	auto reg = f->obterRegistro();
	if (doc.HasMember("entidades") && doc["entidades"].IsArray())
	{
		for (auto& entidade : doc["entidades"].GetArray())
		{
			/// cria entidade
			auto id = reg->criar();
			/// itera os componentes
			if (entidade.HasMember("componentes") && entidade["componentes"].IsArray())
			{
				for (auto& componente : entidade["componentes"].GetArray())
				{
					if (componente.HasMember("tipo") && componente["tipo"].IsString())
					{
						const char* i = componente["tipo"].GetString();
						if (std::strcmp(i, "camera") == 0)
						{
							reg->adicionar<camera>(id, vetor3(0.f, 0.f, 0.f));
							f->definirCamera(id);

							auto arr = componente["posicao"].GetArray();
							f->obterCamera()->posicao = {arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat()};
							if (componente.HasMember("olhar"))
								f->obterCamera()->olhar(componente["olhar"].GetInt());

							if (componente.HasMember("yaw"))
								f->obterCamera()->yaw = componente["yaw"].GetFloat();

							if (componente.HasMember("pitch"))
								f->obterCamera()->pitch = componente["pitch"].GetFloat();

							if (componente.HasMember("escala"))
								f->obterCamera()->escala = componente["escala"].GetFloat();

							if (componente.HasMember("alvo"))
								f->obterCamera()->flag_alvo = componente["alvo"].GetBool();

							if (componente.HasMember("ortho"))
								f->obterCamera()->flag_orth = componente["ortho"].GetBool();

							if (componente.HasMember("ceu"))
							{
								auto ceu = componente["ceu"].GetArray();
								f->obterCamera()->ceu =
								{
									ceu[0].GetFloat() / 255,
									ceu[1].GetFloat() / 255,
									ceu[2].GetFloat() / 255,
									ceu[3].GetFloat() / 255,
								};
							}
						}
						else if (std::strcmp(i, "renderizador") == 0)
						{
							const char* path = componente["diretorio"].GetString();
							reg->adicionar<renderizador>(id, new modelo(path));
							auto render = reg->obter<renderizador>(id.id);

							/// extrai sahder
							if (componente.HasMember("vertex_shader") && componente.HasMember("fragment_shader"))
							{
								render->modelo->definirShader(componente["vertex_shader"].GetString(), componente["fragment_shader"].GetString());
							}
							/// extrai material
							if (componente.HasMember("malhas"))
							{
								auto malhas = componente["malhas"].GetArray();
								for (auto& malha : malhas)
								{
									bubble::malha* m;
									if (malha["id"].IsInt())
									{
										m = render->modelo->obterMalha(malha["id"].GetInt());
										analizarMalha(m, malha);
									}
									else if (malha["id"].IsString())
									{
										for (auto& m : render->modelo->malhas)
										{
											analizarMalha(&m, malha);
										}
									}
								}
							}
						}
						else if (std::strcmp(i, "propriedades") == 0)
							reg->adicionar<propriedades>(id);
						else if (std::strcmp(i, "transformacao") == 0)
						{
							reg->adicionar<transformacao>(id);
							auto tr = reg->obter<transformacao>(id.id);
							auto pos = componente["posicao"].GetArray();
							tr->posicao = { pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat() };
							auto rot = componente["rotacao"].GetArray();
							tr->rotacao = { rot[0].GetFloat(), rot[1].GetFloat(), rot[2].GetFloat() };
							auto esc = componente["escala"].GetArray();
							tr->escala = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };
						}
						else if (std::strcmp(i, "texto") == 0)
						{
							std::string frase{};
							float size{ 1.f };
							if (componente.HasMember("frase") && componente["frase"].IsString())
								frase = componente["frase"].GetString();
							if (componente.HasMember("escala") && componente["escala"].IsFloat())
								size = componente["escala"].GetFloat();
							reg->adicionar<texto>(id, frase, size);
						}
						else if (std::strcmp(i, "codigo") == 0)
						{
							reg->adicionar<codigo>(id, componente["diretorio"].GetString());
						}
						else if (std::strcmp(i, "imagem") == 0)
						{
							reg->adicionar<imagem>(id, componente["diretorio"].GetString());
							reg->obter<imagem>(id.id)->flip = componente["flip"].GetBool();
						}
						else if (std::strcmp(i, "codigo") == 0)
						{
							reg->adicionar<codigo>(id, componente["diretorio"].GetString());
						}
						else if (std::strcmp(i, "fisica") == 0)
						{
							btCollisionShape* forma;
							auto f = componente["forma"].GetString();
							const btVector3 n(0, 1, 0);
							btVector3 posin;
							btScalar massa;

							if (componente.HasMember("posicao_inicial"))
								posin = {componente["posicao_inicial"].GetArray()[0].GetFloat(),componente["posicao_inicial"].GetArray()[1].GetFloat(),componente["posicao_inicial"].GetArray()[2].GetFloat()};
							if (std::strcmp(f, "esfera") == 0)
							{
								forma = new btSphereShape(componente["raio"].GetFloat());
							}
							else if(std::strcmp(f, "plano") == 0)
								forma = new btStaticPlaneShape(n, 0);
							else if(std::strcmp(f, "caixa") == 0)
								forma = new btBoxShape(btVector3(1.f, 1.f, 1.f));
							else if (std::strcmp(f, "modelo") == 0)
							{
								reg->adicionar<fisica>(id, true, massa, posin);
								continue;
							}
							massa = componente["massa"].GetFloat();
							reg->adicionar<fisica>(id, forma, massa, posin);
						}
					}
				}
			}
		}
	}
}

void bubble::fase::analizar(const char* diretorio)
{
	std::ifstream file(diretorio);
	std::stringstream sb;
	sb << file.rdbuf();
	Document doc;
	doc.Parse(sb.str().c_str());

	if (doc.HasParseError()) 
	{
		debug::emitir(Erro, "Parse da fase");
	}
	/*----Analise da cena-----*/
	if (doc.HasMember("nome") && doc["nome"].IsString())
	{
		_Mnome = doc["nome"].GetString();
		debug::emitir("Fase", "Nome definido como " + _Mnome);
	}
	if (doc.HasMember("selecionada") && doc["selecionada"].IsBool())
	{
		if (doc["selecionada"].GetBool())
		{
			debug::emitir("Fase", "Fase ativa");
		}
	}
	/*------------------------*/
	analizarEntidades(doc, this);
}

void bubble::fase::pausar()
{
	debug::emitir("fase", "Pausando");
	rodando = false;
	scodigo.pararThread();
	sfisica.pararThread();
}

void bubble::fase::parar()
{
	debug::emitir("fase", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
	scodigo.pararThread();
	sfisica.pararThread();
}

void bubble::fase::iniciar()
{
	debug::emitir("fase", "Iniciando");
	if (rodando != false)
		return;

	/// o sistema de código apenas inicia ao começar a fase
	/// no modo de joo
	scodigo.inicializar(this);
	sfisica.inicializar(this);

	// capturar snapshot do registro
	rodando = true;
	scodigo.iniciarThread();
	sfisica.iniciarThread();
}

double elapsedTime;

void bubble::fase::atualizar(double deltaTime)
{
	srender.atualizar(deltaTime);
	sinterface.atualizar(deltaTime);
}

void bubble::fase::definirCamera(const entidade& ent)
{
	if (reg.tem<camera>(ent.id))
		camera_atual = reg.obter<camera>(ent.id);
}

std::shared_ptr<bubble::camera> bubble::fase::obterCamera() const
{
	return camera_atual;
}

bubble::registro* bubble::fase::obterRegistro()
{
	return &reg;
}

std::string bubble::fase::nome() const
{
	return _Mnome;
}
