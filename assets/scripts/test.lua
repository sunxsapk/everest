__serialize = {
    speed = Types.float,
}

speed = 13

OnCreate = function()
    rigidbody = entity:get_rigidbody2d()
    position = entity:get_transform().position
    spr = entity:get_spriteRenderer()
end

OnUpdate = function(_)
    if spr ~= nil then
        spr.color = vec4.new(math.abs(math.sin(Time.getTime())), math.abs(math.cos(Time.getTime())), 0, 1)
    end

    if rigidbody == nil then return end

    local mp = Input.getMousePosition()
    mp = Scene.screenToWorld(mp)

    local vel = vec2.new(mp.x - position.x, mp.y - position.y)
    rigidbody:addForce(vel:smul(speed), ForceMode.Force)
end

OnCollision = function(data)
    -- data.other:remove_rigidbody2d()
    -- print(entity:get_tag().tag, data.other:get_tag().tag)
end
