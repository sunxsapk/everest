#include "math/random.h"

namespace Everest {
    std::mt19937 Random::_engine;
    std::uniform_int_distribution<std::mt19937::result_type> _dist;
}
