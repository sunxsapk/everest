SERIALIZE = {
    target = Types.Entity,
    speed = Types.float
}

target = Entity.new()
speed = 10

OnCreate = function()
    rigidbody = entity:get_rigidbody2d()
    position = entity:get_transform().position
    tpos = target:get_transform().position

    local esc = target:get_scripts()
    local tst = esc:get("SerializeTest")
    if tst == nil then return end
    local args = { caller = entity:get_tag().tag }
    tst:call("testFunction", args)
end

testFunction = function(args)
    print("caller", args.caller, "callee", entity:get_tag().tag)
    return args
end

OnUpdate = function(_)
    if rigidbody == nil then return end

    local vel = vec2.new(tpos.x - position.x, tpos.y - position.y)
    rigidbody:addForce(vel:smul(speed), ForceMode.Force)
end
