SERIALIZE = {
    GameManager = Types.Entity
}

GameManager = nil


OnCreate = function ()
    local gms = GameManager:get_scripts()
    gm = gms:get("GameManager")
end


OnCollision = function (data)
    local tag = data.other:get_tag().name
    if tag == "red" then
        print("Red Ball in hole")
        Scene.destroyEntity(data.other)
    elseif tag == "yellow" then
        print("Yellow Ball in hole")
        Scene.destroyEntity(data.other)
    elseif tag == "black" then
        print("Black Ball in hole")
        Scene.destroyEntity(data.other)
    elseif tag == "white" then
        print("White Ball in hole")
        Scene.destroyEntity(data.other)
    end
end
