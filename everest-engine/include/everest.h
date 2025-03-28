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
#include "core/guilayer.h"
#include "core/layer.h"
#include "core/window.h"
#include "core/input.h"
#include "core/keycode.h"
#include "core/time.h"
#include "core/utils.h"
#include "core/uifontmanager.h"
#include "utils/assetsManager.h"
#include "utils/uuid.h"

#include "events/event.h"
#include "events/keyevent.h"
#include "events/mouseevent.h"
#include "events/windowevent.h"

#include "math/types.h"
#include "math/utils.h"
#include "math/random.h"

#include "utils/utils.h"

#include "renderer/vao.h"
#include "renderer/shader.h"
#include "renderer/renderer.h"
#include "renderer/renderer2d.h"
#include "renderer/texture.h"
#include "renderer/sprite.h"
#include "renderer/framebuffer.h"

#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/components.h"
#include "scene/camera.h"
#include "scene/yaml_extensions.h"
#include "scene/serializer.h"
#include "core/project_serializer.h"
#include "scene/scenemanager.h"

#include "scripting/evscript.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "core/entry.h"
