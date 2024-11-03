/* Behold !!!!!!!!!!!!
 * This is the main include file for the everest engine. It contains everything
 * that a user needs to use this engine.
 */

#pragma once

#include "pch.h"

#include "core/core.h"
#include "core/log.h"
#include "core/application.h"
#include "core/dbghelper.h"
#include "core/debuglayer.h"
#include "core/layer.h"
#include "core/window.h"
#include "core/input.h"
#include "core/keycode.h"
#include "core/time.h"

#include "events/event.h"
#include "events/keyevent.h"
#include "events/mouseevent.h"
#include "events/windowevent.h"

#include "math/types.h"
#include "math/utils.h"

#include "renderer/vao.h"
#include "renderer/shader.h"
#include "renderer/renderer.h"

#include "scene/camera.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

