#pragma once
#include "TypeMacros.hpp"
#include "HostedGameData.hpp"
#include <vector>

DECLARE_JSON_CLASS(ServerBrowser::Core, ServerBrowserResult,
    GETTER_VALUE(int, count);
    GETTER_VALUE(int, offset);
    GETTER_VALUE(int, limit);
    GETTER_VALUE(std::vector<ServerBrowser::Core::HostedGameData>, lobbies);
    GETTER_VALUE(std::string, message);
)
