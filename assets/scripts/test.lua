OnCreate = function()
    pos = entity:get_transform().position
end

OnUpdate = function(dt)
    pos.y = pos.y + 5 * dt
end
