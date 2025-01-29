-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local config = require("config")

local s = config.padding + config.escalaBorda
function iniciar()
	eu.imagem.limite = vetor2d(0,0)
end
function atualizar()
	eu.imagem:definirID(entidade(5).camera.framebuffer)
	eu.imagem.padding = vetor2d(s, s)
	eu.imagem.limite.x = util.lerp(eu.imagem.limite.x, inputs:tamanhoTela().x - config.padding*2 - config.escalaBorda*2, 0.1)
	eu.imagem.limite.y = util.lerp(eu.imagem.limite.y, inputs:tamanhoTela().y - config.padding - config.escalaCima*2, 0.1)
	entidade(5).camera:viewport(vetor2d(inputs:tamanhoTela().x,inputs:tamanhoTela().y))
end