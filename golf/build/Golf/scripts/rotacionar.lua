local angulo = 0
local velo = 10
function atualizar()
	angulo = tempo.obterDeltaTime() * velo
	eu.transformacao.rotacao.y = eu.transformacao.rotacao.y+angulo
end