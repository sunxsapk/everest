/*
 * ========== UUID / GUID ==========
 * Author: Sunil Sapkota
 * Description: Universally/Globally Unique Identifiers (UUID / GUID)s are essential for giving
 * unique IDs to assets, entities, and other engine-essentials. These are important for handling
 * assets, referencing entities, etc. UUID class implements the concept of unique identifiers for 
 * everest-engine
 */


#pragma once


#include "math/types.h"

namespace Everest {

    class UUID {
         public:

             UUID();
             UUID(u64 uuid);

             operator u64() const {return _uuid;}
         private:
             u64 _uuid;
    };
}


namespace std {
    template<>
    struct hash<Everest::UUID>{
        std::size_t operator()(const Everest::UUID& uuid) const {
            return hash<u64>()((u64)uuid);
        }
    };
}
