OnCreate = function()
    -- pos = entity:get_transform().position
    rigidbody = entity:get_rigidbody2d()
    spr = entity:get_spriteRenderer()
end

OnUpdate = function(dt)
    if spr ~= nil then
        spr.color = vec4.new(math.abs(math.sin(Time.getTime())), 0, 0, 1)
    end

    if rigidbody == nil then return end

    local vel = Input.mouseScroll()
    local mag = math.abs(vel:dot(vel))
    if mag > 0 then
        vel:smul(80 * dt)
        rigidbody.velocity:add(vel)
    end

    if Input.getKeyDown(Keycode.K_space) then
        rigidbody:addForce(vec2.new(0, 400), ForceMode.Force)
    end

    local ent = Scene.getMainCameraEntity()
    local mc = ent:get_camera()
    if mc ~= nil then
        print("Camera aspect ", mc:getAspect())
        mc:setAspect(16 / 9 * math.abs(math.sin(Time.getTime())))
    else
        print("Main Camera is nil", ent:has_camera())
    end

end

OnCollision = function(data)
    data.other:remove_rigidbody2d()
    -- print(entity:get_tag().tag, data.other:get_tag().tag)
end
