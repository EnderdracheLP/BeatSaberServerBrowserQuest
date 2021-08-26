#include "API/ServerBrowser/SemVer.hpp"

DESERIALIZE_METHOD(ServerBrowser::API, SemVer,
    DESERIALIZE_VALUE(Major, major, Int)
    DESERIALIZE_VALUE(Minor, Offset, Int)
    DESERIALIZE_VALUE(Build, build, Int)
    DESERIALIZE_VALUE_OPTIONAL(Revision, revision, Int)
)