__serialize = {
    target = Types.Entity,
    speed = Types.float
}

target = Entity.new()
speed = 10

OnCreate = function()
    rigidbody = entity:get_rigidbody2d()
    position = entity:get_transform().position
    tpos = target:get_transform().position
end

OnUpdate = function(_)
    if rigidbody == nil then return end

    local vel = vec2.new(tpos.x - position.x, tpos.y - position.y)
    rigidbody:addForce(vel:smul(speed), ForceMode.Force)
end
