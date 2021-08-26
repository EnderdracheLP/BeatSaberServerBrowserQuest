#pragma once
#include "../TypeMacros.hpp"
#include "Lobby.hpp"

#include <vector>

DECLARE_JSON_CLASS(ServerBrowser::API, Page,
    GETTER_VALUE(int, count);
    GETTER_VALUE(int, offset);
    GETTER_VALUE(int, limit);
    GETTER_VALUE(std::vector<ServerBrowser::API::Lobby>, lobbies);
    //GETTER_VALUE_OPTIONAL();
    GETTER_VALUE(std::string, message);
)
