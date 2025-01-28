-- Autor Daniel Oliveira (c) 2025 --

function iniciar()
    print("Iniciando o jogo...")
end

function atualizar()
    -- Suaviza a rotação da câmera com base no movimento do mouse
    local sensibilidade = 0.1
    eu.transformacao.rotacao.y = eu.transformacao.rotacao.y + inputs:mouse().x * sensibilidade
    eu.transformacao.rotacao.x = eu.transformacao.rotacao.x - inputs:mouse().y * sensibilidade
    
    -- Vetor de direção baseado na rotação da câmera
    local direcao_frente = eu.camera:frente()
    local direcao_direita = eu.camera:direita()
    local velocidade = 0.5
    
    -- Movimento para frente e para trás
    if inputs.pressionada("W") then
        eu.camera:mover(direcao_frente * velocidade)
    end
    if inputs.pressionada("S") then
        eu.camera:mover(direcao_frente * -velocidade)
    end
    
    -- Movimento lateral
    if inputs.pressionada("A") then
        eu.camera:mover(direcao_direita * -velocidade)
    end
    if inputs.pressionada("D") then
        eu.camera:mover(direcao_direita * velocidade)
    end
end
