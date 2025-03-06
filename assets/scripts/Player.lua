SERIALIZE = {
    power = Types.float
}


power = 10
md = false
lastMousePos = vec2.new(0, 0)

OnCreate = function ()
    rigidbody = entity:get_rigidbody2d()
end

OnUpdate = function (_)
    if rigidbody.velocity:length() > 1 then
        return
    end

    if Input.getMouseButtonDown(MouseButton.M_0) and not md then
        md = true
        lastMousePos = Input.getMousePosition()
    elseif Input.getMouseButtonUp(MouseButton.M_left) and md then
        md = false
        local dp = Scene.screenToWorld(lastMousePos)
        dp:sub(Scene.screenToWorld(Input.getMousePosition()))
        if rigidbody ~= nil then
            local f = vec2.new(dp.x * power, dp.y * power)
            rigidbody:addForce(f, ForceMode.Impulse)
        end
    end
end
