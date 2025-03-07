SERIALIZE = {
    power = Types.float,
    marker = Types.Entity
}


power = 8
marker = Entity.new()

md = false
lastMousePos = vec2.new(0, 0)

OnCreate = function()
    rigidbody = entity:get_rigidbody2d()
    transform = entity:get_transform()
    m_tfr = marker:get_transform()
    m_spr = marker:get_spriteRenderer()
    m_spr.active = false
end

OnUpdate = function(_)
    m_spr.active = false

    if rigidbody.velocity:length() > 1 then
        return
    end

    if Input.getMouseButtonDown(MouseButton.M_0) and not md then
        md = true
        lastMousePos = Input.getMousePosition()
    elseif md then
        local dp = Scene.screenToWorld(lastMousePos)
        dp:sub(Scene.screenToWorld(Input.getMousePosition()))

        if m_tfr ~= nil then
            m_tfr.position = transform.position
            m_tfr.rotation.z = math.deg(math.atan(dp.y, dp.x))
            m_tfr.scale.x = dp:length() * 2 + 0.1
        end
        m_spr.active = true

        if Input.getMouseButtonUp(MouseButton.M_left) then
            md = false
            if rigidbody ~= nil then
                local f = vec2.new(dp.x * power, dp.y * power)
                rigidbody:addForce(f, ForceMode.Impulse)
            end
        end
    else
        m_spr.active = false
    end
end
