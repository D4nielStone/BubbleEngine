local bola
local buraco
local cam
local posantiga
local forcaAtrativa = 20  -- Intensidade da força atrativa
local venceu = false
local breack = false
local escalaAntiga

function iniciar()
    bola = entidade(1)
    buraco = entidade(7)
    cam = entidade(5)
    posantiga = eu.transformacao.posicao.y
    escalaAntiga = cam.camera.escala
end

function atualizar()
    -- Verifica se a bola está perto da posição desejada para ajustar a altura
    if util.distanciaV2(vetor2d(bola.transformacao.posicao.x, bola.transformacao.posicao.z), vetor2d(eu.transformacao.posicao.x, eu.transformacao.posicao.z)) < 3 then
        eu.transformacao.posicao.y = util.lerp(eu.transformacao.posicao.y, posantiga + 4, 0.1)
    else
        eu.transformacao.posicao.y = util.lerp(eu.transformacao.posicao.y, posantiga, 0.1)
    end

    -- Verifica se a bola está perto do buraco
    if util.distanciaV2(vetor2d(bola.transformacao.posicao.x, bola.transformacao.posicao.z), vetor2d(buraco.transformacao.posicao.x, buraco.transformacao.posicao.z)) < 1 then
        -- Aplica uma força de atração
        local direcao = vetor3(buraco.transformacao.posicao.x - bola.transformacao.posicao.x, buraco.transformacao.posicao.y - bola.transformacao.posicao.y, buraco.transformacao.posicao.z - bola.transformacao.posicao.z)
        direcao = util.normalizarV3(direcao)
        bola.fisica:aplicarForca(vetor3(direcao.x * forcaAtrativa,direcao.y* forcaAtrativa,direcao.z* forcaAtrativa))
        if util.distanciaV3(buraco.transformacao.posicao, bola.transformacao.posicao) < 0.5
            then 
                venceu = true
            end
    end

    if venceu then
        if cam.camera.escala > 0.1 then
            cam.camera.escala = util.lerp(cam.camera.escala, 0, 0.05)
        else if not breack then
            faseAtual:pausar()
            breack = true
        end
    end
    
    end
end