#include "objeto_geral.h"
#include "src/componentes/renderizador.h"
#include "src/componentes/transformador.h"

void Bubble::Entidades::ObjetoGeral::carregarModelo(Bubble::Arquivadores::Arquivo3d object_file) {
	Componentes.push_back(std::make_unique<Bubble::Componentes::Transformador>());
	int m = 0;
	for (auto vertex : object_file.vertices) {
		Componentes.push_back(std::make_unique<Bubble::Componentes::Rendererizador>(vertex, object_file.materiais[m]));
		m++;
	}
}