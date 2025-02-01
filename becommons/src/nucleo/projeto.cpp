#include "nucleo/projeto.hpp"
#include "nucleo/fase.hpp"
#include "os/janela.hpp"
#include "filesystem"
#include "string"
#include "iostream"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h> 
bubble::projeto::projeto(const std::string &diretorio) : diretorioDoProjeto(diretorio)
{
    projeto_atual = this;
    std::string full_path = diretorio + "/config";
    if(!std::filesystem::exists(full_path)) 
    {
        debug::emitir(Erro, "Arquivo de projeto não encontrado!");
        return;
    }
    std::ifstream file(full_path);
    std::stringstream sb;
    
    sb << file.rdbuf();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    /*      ERROS     */
    if(doc.HasParseError()) 
    {
        debug::emitir(Erro, "Parse do projeto!");
        return;
    }

    if(!doc.HasMember("lancamento") || !doc["lancamento"].IsString())
    {
        debug::emitir(Erro, "Defina fase de lancamento em config!");
        return;
    }
    if(!doc.HasMember("janela") || !doc["janela"].IsObject())
    {
        debug::emitir(Erro, "Defina janela em config!");
        return;
    }
    if(!doc["janela"].GetObject().HasMember("largura") || !doc["janela"].GetObject()["largura"].IsInt())
    {
        debug::emitir(Erro, "Defina largura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("altura") || !doc["janela"].GetObject()["altura"].IsInt())
    {
        debug::emitir(Erro, "Defina altura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("titulo") || !doc["janela"].GetObject()["titulo"].IsString())
    {
        debug::emitir(Erro, "Defina titulo da janela em config!");
        return;
    }    
    if(!doc["janela"].GetObject().HasMember("icone") || !doc["janela"].GetObject()["icone"].IsString())
    {
        debug::emitir(Erro, "Defina icone da janela em config!");
        return;
    }   
    /*              */


    const char* nome_janela = doc["janela"].GetObject()["titulo"].GetString();
    std::string icon_path = doc["janela"].GetObject()["icone"].GetString();

    instanciaJanela = new bubble::janela(nome_janela,
     vet2(doc["janela"].GetObject()["largura"].GetInt(), doc["janela"].GetObject()["altura"].GetInt()),
    (diretorio + "/" + icon_path).c_str());

	projeto_atual->fase_atual = new bubble::fase(diretorio + "/" + doc["lancamento"].GetString() + ".fase");
	projeto_atual->fase_atual->iniciar();
}

void bubble::projeto::rodar()
{
    while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		projeto_atual->fase_atual->atualizar(0.01666);

		instanciaJanela->swap();
	}
}
void bubble::projeto::fase(const std::string &nome)
{
    bubble::file_de_tarefas.push([this, nome]()
    {
    projeto_atual->fase_atual->parar();
    delete projeto_atual->fase_atual;
    projeto_atual->fase_atual = new bubble::fase(diretorioDoProjeto + "/" + nome + ".fase");
	projeto_atual->fase_atual->iniciar();
    }
    );
}
bubble::fase * bubble::projeto::obterFaseAtual()
{
return fase_atual;
}