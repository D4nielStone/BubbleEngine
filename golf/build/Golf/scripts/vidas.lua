local cam
local pos_inicio
local bola

function atualizar()
	bola.fisica:defPosicao(pos_inicio)
end
function iniciar()
	eu.transformacao.escala = vetor3(0,0,0)
	cam = entidade(5).camera
	bola = entidade(1)
	pos_inicio = bola.transformacao.posicao
end