OnCreate = function()
    -- pos = entity:get_transform().position
    rigidbody = entity:get_rigidbody2d()
    spr = entity:get_spriteRenderer()
    TotalTime = 0.0
end

OnUpdate = function(dt)
    TotalTime = TotalTime + dt

    if spr ~= nil then
        spr.color = vec4.new(math.abs(math.sin(TotalTime)), 0, 0, 1)
    end

    if rigidbody == nil then return end

    local force = Input.getAxis()
    force:smul(100)

    rigidbody:addForce(force, ForceMode.Force)
end

OnCollision = function(data)
    -- entity:remove_rigidbody2d()
    -- pBeginrint(entity:get_tag().tag, data.other:get_tag().tag)
    print("Collision")
    data.other:destroy()
end
