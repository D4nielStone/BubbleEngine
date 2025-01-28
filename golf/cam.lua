-- Autor Daniel Oliveira (c) 2025 --
local sensibilidade = 0.2
local velocidade = 0.5
local deltaX
local deltaY

function iniciar()
    print("Iniciando o jogo...")
end

function atualizar()
        deltaX = inputs:mouse().x - inputs:tamanhoTela().x/2
        deltaY = inputs:mouse().y - inputs:tamanhoTela().y/2
        
        
    -- Suaviza a rotação da câmera com base no movimento do mouse
    eu.transformacao.rotacao.y = eu.transformacao.rotacao.y + deltaX* sensibilidade
    eu.transformacao.rotacao.x = eu.transformacao.rotacao.x - deltaY* sensibilidade
    
    inputs.cursor(inputs:tamanhoTela().x/2,inputs:tamanhoTela().y/2)
    
    
    -- Movimento para frente e para trás
    if inputs.pressionada("W") then
        eu.camera:mover(vetor3(0, 0, velocidade))
    end
    if inputs.pressionada("S") then
        eu.camera:mover(vetor3(0, 0, -velocidade))
    end
    
    -- Movimento lateral
    if inputs.pressionada("A") then
        eu.camera:mover(vetor3(-velocidade, 0, 0))
    end
    if inputs.pressionada("D") then
        eu.camera:mover(vetor3(velocidade, 0, 0))
    end
end
