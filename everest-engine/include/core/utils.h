/*
 * ======== Utils ==========
 * Author : Sunil Sapkota
 * Description : Contains some utility classes, definitions, functions,
 * constants, etc.
 */

#pragma once
#include <memory>

#define BIT(x) (1<<x)

namespace Everest {
    template<typename T>
    using scope = std::unique_ptr<T>;

    template<typename T>
    using ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    ref<T> createRef(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    scope<T> createScope(Args&&... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

