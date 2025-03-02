__serialize = {
    speed = Types.float,
    name = Types.string,
    num = Types.int,
    box = Types.vec2,
    pos = Types.vec3,
    cvec4 = Types.vec4,
    color = Types.color
}

speed = 10
name = "boo"
num = 10
box = vec2.new(1, 2)
pos = vec3.new(1123, 223, 2453)
cvec4 = vec4.new(34.2, 23.3, .23, 1.1234)
color = vec4.new(1, 0, 0, 1)


OnCreate = function()
    print("OnCreate")
end

OnUpdate = function(_)
end


OnCollision = function(data)
    print("OnCollision ", data.other:get_tag().tag)
end
