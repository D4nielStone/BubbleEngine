-- Autor Daniel Oliveira (c) 2025 --
function iniciar()
    print("Iniciando o jogo...")
end
function atualizar()
    eu.transformacao.rotacao.y = inputs:mouse().x
    if inputs.pressionada("W") then
    eu.camera:mover(vetor3(0, 0, 0.5))
    end
    if inputs.pressionada("S") then
        eu.camera:mover(vetor3(0, 0, -0.5))
        end
    print("A")
end
