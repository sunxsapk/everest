#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <algorithm>
#include <array>
#include <functional>

#include "core/instrumentor.h"
#include "core/utils.h"

#include "math/types.h"
#include "math/utils.h"

#include "core/log.h"
#include "core/dbghelper.h"

#ifdef WIN32
#include <Windows.h>
#endif
