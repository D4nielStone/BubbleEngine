#include "fase.hpp"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <src/componentes/codigo.hpp>
#include <src/componentes/texto.hpp>
#include <src/componentes/transformacao.hpp>
#include <src/componentes/propriedades.hpp>
#include <src/componentes/renderizador.hpp>
#include <iostream>
#include <os/janela.hpp>

using namespace rapidjson;

bubble::fase::fase() : _Mnome("")
{
	srender.inicializar(this);
	sinterface.inicializar(this);
	fase_atual = this;
}

bubble::fase::fase(const char* diretorio) : diretorio(diretorio)
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

void bubble::fase::analizar(const char* diretorio)
{
	std::ifstream file(diretorio);
	std::stringstream sb;
	sb << file.rdbuf();
	Document doc;
	doc.Parse(sb.str().c_str());

	if (doc.HasParseError()) 
	{
		Debug::emitir(Erro, "Parse da fase");
	}
	if (doc.HasMember("nome") && doc["nome"].IsString())
	{
		_Mnome = doc["nome"].GetString();
		Debug::emitir("Fase", "Nome definido como " + _Mnome);
	}
	if (doc.HasMember("selecionada") && doc["selecionada"].IsBool())
	{
		if (doc["selecionada"].GetBool())
		{
			Debug::emitir("Fase", "Fase ativa");
		}
	}
	if (doc.HasMember("entidades") && doc["entidades"].IsArray())
	{
		int id{ 0 };
		for (auto& entidade : doc["entidades"].GetArray())
		{
			/// cria entidade
			auto id = reg.criar();
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
							reg.adicionar<camera>(id, vetor3(0.f, 0.f, 0.f));
							definirCamera(id);

							auto arr = componente["posicao"].GetArray();
							camera_atual->posicao = { arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat()};
							if(componente.HasMember("olhar"))
							obterCamera()->olhar(componente["olhar"].GetInt());
							
							if(componente.HasMember("yaw"))
							obterCamera()->yaw = componente["yaw"].GetFloat();

							if(componente.HasMember("pitch"))
							obterCamera()->pitch = componente["pitch"].GetFloat();

							if(componente.HasMember("escala"))
							obterCamera()->escala = componente["escala"].GetFloat();
							if (componente.HasMember("ceu"))
							{
								auto ceu = componente["ceu"].GetArray();
								obterCamera()->ceu = 
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
							reg.adicionar<renderizador>(id, new modelo(path));
							auto render = reg.obter<renderizador>(id.id);

							/// extrai material
							if (componente.HasMember("malhas"))
							{
								auto malhas = componente["malhas"].GetArray();
								for (auto& malha : malhas)
								{
									auto m = render->modelo->obterMalha(malha["id"].GetInt());
									/// cor difusa
									m->material.difusa = 
									{
										malha["cor_difusa"].GetArray()[0].GetFloat() / 255,
										malha["cor_difusa"].GetArray()[1].GetFloat() / 255,
										malha["cor_difusa"].GetArray()[2].GetFloat() / 255,
										malha["cor_difusa"].GetArray()[3].GetFloat() / 255,
									};
									/// recebe luz
									m->material.recebe_luz = malha["recebe_luz"].GetBool();
								}
							}
						}
						else if (std::strcmp(i, "propriedades") == 0)
							reg.adicionar<propriedades>(id);
						else if (std::strcmp(i, "transformacao") == 0)
						{
							reg.adicionar<transformacao>(id);
							auto tr = reg.obter<transformacao>(id.id);
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
							reg.adicionar<texto>(id, frase, size);
						}
						else if (std::strcmp(i, "codigo") == 0)
						{
							// reg.adicionar<codigo>(id);
						}
					}
				}
			}
		}
	}
}

void bubble::fase::pausar()
{
	Debug::emitir("fase", "Pausando");
	rodando = false;
	scodigo.pararThread();
}

void bubble::fase::parar()
{
	Debug::emitir("fase", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
	scodigo.pararThread();
}

void bubble::fase::iniciar()
{
	Debug::emitir("fase", "Iniciando");
	if (rodando != false)
		return;

	/// o sistema de código apenas inicia aao começar a fase
	/// no modo de joo
	scodigo.inicializar(this);

	// capturar snapshot do registro
	rodando = true;
	scodigo.iniciarThread();
}

double elapsedTime;

void bubble::fase::atualizar(double deltaTime)
{
	if (rodando == false)
	{
		// se parado apenas renderiza
		srender.atualizar(deltaTime);
		sinterface.atualizar(deltaTime);
	}
	else
	{
		// efetua operacoes matematicas de transformacao
		elapsedTime += deltaTime;
		srender.atualizar(deltaTime);
		sinterface.atualizar(deltaTime);
	}
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
