#include "utils/uuid.h"


#include <random>

namespace Everest {

    static std::random_device _randomDevice;
    static std::mt19937_64 _engine(_randomDevice());
    static std::uniform_int_distribution<u64> _uniformDist;

    UUID::UUID(): _uuid(_uniformDist(_engine)){ }


    UUID::UUID(u64 uuid) : _uuid(uuid){ }
}
