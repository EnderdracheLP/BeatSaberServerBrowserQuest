#pragma once
#include "GlobalNamespace/ConnectionFailedReason.hpp"
#include <string>

namespace ServerBrowser::Utils {
    class ConnectionErrorText {
    public:
        static std::string Generate(GlobalNamespace::ConnectionFailedReason reason);
    };
}