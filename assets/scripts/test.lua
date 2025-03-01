OnCreate = function()
    -- pos = entity:get_transform().position
    rigidbody = entity:get_rigidbody2d()
    position = entity:get_transform().position
    spr = entity:get_spriteRenderer()
end

OnUpdate = function(_)
    if spr ~= nil then
        spr.color = vec4.new(math.abs(math.sin(Time.getTime())), 0, 0, 1)
    end

    if rigidbody == nil then return end

    local mp = Input.getMousePosition()
    mp = Scene.screenToWorld(mp)

    local pos = Scene.screenToWorld(Scene.worldToScreen(position))

    local vel = vec2.new(mp.x - pos.x, mp.y - pos.y)
    rigidbody:addForce(vel:smul(10), ForceMode.Force)

    if Input.getKeyDown(Keycode.K_space) then
        rigidbody:addForce(vec2.new(0, 400), ForceMode.Force)
    end
end

OnCollision = function(data)
    -- data.other:remove_rigidbody2d()
    print(entity:get_tag().tag, data.other:get_tag().tag)
end
