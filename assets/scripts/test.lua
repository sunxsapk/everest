OnCreate = function()
    -- pos = entity:get_transform().position
    rigidbody = entity:get_rigidbody2d()
    spr = entity:get_spriteRenderer()
    TotalTime = 0.0
end

OnUpdate = function(dt)
    TotalTime = TotalTime + dt

    if(rigidbody ~= nil) then
        rigidbody:addForce(vec2.new(0, 10), ForceMode.Force)
    end

    if spr ~= nil then
        spr.color = vec4.new(math.abs(math.sin(TotalTime)), 0, 0, 1)
    end
end
