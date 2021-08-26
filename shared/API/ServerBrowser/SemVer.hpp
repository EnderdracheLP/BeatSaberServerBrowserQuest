#pragma once
#include "../TypeMacros.hpp"

DECLARE_JSON_CLASS(ServerBrowser::API, SemVer,
    GETTER_VALUE(int, Major);
    GETTER_VALUE(int, Minor);
    GETTER_VALUE(int, Build);
    GETTER_VALUE_OPTIONAL(int, Revision);
)