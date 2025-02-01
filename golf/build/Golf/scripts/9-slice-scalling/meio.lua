-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local config = require("config")
local parent_path = script_path:match("(.*/).*/")
package.path = package.path .. ";" .. parent_path .. "?.lua"
local gamestate = require("gamestate")

local s = config.padding + config.escalaBorda
function iniciar()
	eu.imagem.limite = vetor2d(1,0)
	eu.imagem.padding = vetor2d(inputs:tamanhoTela().x/2,inputs:tamanhoTela().y/2)
	if projetoAtual:faseAtual():nome() == "Tutorial" then
		gamestate.write()
		
	end
end
function atualizar()
	gamestate.load()
	eu.imagem:definirID(entidade(5).camera.framebuffer)
	eu.imagem.padding = vetor2d(inputs:tamanhoTela().x/2 - eu.imagem.limite.x/2, s)
	if(gamestate.won) then
		eu.imagem.limite.x = util.lerp(eu.imagem.limite.x, 0, 0.2)
	else
		eu.imagem.limite.x = util.lerp(eu.imagem.limite.x, inputs:tamanhoTela().x - config.padding*2 - config.escalaBorda*2, 0.2)
	end
	eu.imagem.limite.y = inputs:tamanhoTela().y - config.padding - config.escalaCima*2
	entidade(5).camera:viewport(vetor2d(eu.imagem.limite.x + eu.imagem.padding.x*2,eu.imagem.limite.y + eu.imagem.padding.y*2))
	if eu.imagem.limite.x < 0.5 then
		gamestate.level = gamestate.level + 1
    	gamestate.won = false
		gamestate.write()
        projetoAtual:abrirFase("level" .. tostring(gamestate.level))
	end
end