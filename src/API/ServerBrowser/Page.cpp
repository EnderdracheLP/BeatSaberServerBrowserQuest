#include "API/ServerBrowser/Page.hpp"

DESERIALIZE_METHOD(ServerBrowser::API, Page,
    DESERIALIZE_VALUE(count, Count, Int)
    DESERIALIZE_VALUE(offset, Offset, Int)
    DESERIALIZE_VALUE(limit, Limit, Int)
    DESERIALIZE_VECTOR(lobbies, Lobbies, ServerBrowser::API::Lobby)
    //DESERIALIZE_VALUE_OPTIONAL()
    DESERIALIZE_VALUE(message, Message, String)
)