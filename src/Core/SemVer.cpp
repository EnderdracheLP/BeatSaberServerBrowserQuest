#include "Core/SemVer.hpp"

DESERIALIZE_METHOD(ServerBrowser::Core, SemVer,
    DESERIALIZE_VALUE(Major, major, Int)
    DESERIALIZE_VALUE(Minor, minor, Int)
    DESERIALIZE_VALUE(Build, build, Int)
    DESERIALIZE_VALUE_OPTIONAL(Revision, revision, Int)
)

//SERIALIZE_STRING_METHOD(ServerBrowser::Core, SemVer,
//    SERIALIZE_VALUE(Major, major)
//    SERIALIZE_VALUE(Minor, minor)
//    SERIALIZE_VALUE(Build, build)
//    SERIALIZE_VALUE_OPTIONAL(Revision, revision)
//)