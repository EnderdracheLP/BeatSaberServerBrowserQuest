#pragma once
#include "GlobalNamespace/ConnectionFailedReason.hpp"
#include <string>

namespace ServerBrowser::Utils {
    class ConnectionErrorText {
    public:
        static const std::string Generate(GlobalNamespace::ConnectionFailedReason reason);
    };
}