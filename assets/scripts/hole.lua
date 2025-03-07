SERIALIZE = {
    GameManager = Types.Entity
}

GameManager = nil


OnCreate = function ()
    local gms = GameManager:get_scripts()
    if gms == nil then return end

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
        data.other:get_transform().position = vec3.new(-2, 0.5, 0)
        local rb = data.other:get_rigidbody2d()
        rb.velocity = vec2.new(0)
        rb.angularVelocity = 0
        -- Scene.destroyEntity(data.other)
    end
end
