#pragma once
#include "GlobalNamespace/ConnectionFailedReason.hpp"
#include <string>

namespace ServerBrowser::Utils {
    struct ConnectionErrorText {
        static StringW Generate(GlobalNamespace::ConnectionFailedReason reason);
    };
}