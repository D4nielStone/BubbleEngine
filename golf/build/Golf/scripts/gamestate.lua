-- gamestate.lua
GameState = {
    won = false,
    lost = false,
    score = 0,
    level = 0
}

function GameState.setWin()
    GameState.won = true
end

function GameState.setLose()
    GameState.lost = true
end

function GameState.addScore(points)
    GameState.score = GameState.score + points
end

function GameState.write()
    file = io.open("golfgamestate.txt", "w")
    file:write(
        string.format(
        "won=%s\nlost=%s\nscore=%d\nlevel=%d\n",
        tostring(GameState.won),
        tostring(GameState.lost),
        GameState.score,
        GameState.level)
    )
    file:close()
end

function GameState.load()
    -- Verifica se o arquivo existe
    local file = io.open("golfgamestate.txt", "r")
    if file then
        -- Se o arquivo existir, carrega os dados
        for line in io.lines("golfgamestate.txt") do
            local key, value = line:match("([^=]+)=([^=]+)")
            if key and value then
                -- Tenta converter para número, caso contrário, verifica se é booleano
                if value == "true" then
                    GameState[key] = true
                elseif value == "false" then
                    GameState[key] = false
                else
                    GameState[key] = tonumber(value) or value
                end
            end
        end
        file:close()
    else
        -- Se o arquivo não existir, chama a função write para criar o arquivo
        GameState.write()
    end
end



return GameState