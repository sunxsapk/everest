#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "math/types.h"

#include <iostream>
#include <sstream>
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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "core/core.h"
#include "core/log.h"
#include "core/dbghelper.h"
#include "core/input.h"
#include "core/time.h"

#ifdef WIN32
#include <Windows.h>
#endif
