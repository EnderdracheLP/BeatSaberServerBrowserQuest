#pragma once
#include "TypeMacros.hpp"

DECLARE_JSON_CLASS(ServerBrowser::Core, SemVer,
    JSON_VALUE(int, Major);
    JSON_VALUE(int, Minor);
    JSON_VALUE(int, Build);
    JSON_VALUE_OPTIONAL(int, Revision);
)