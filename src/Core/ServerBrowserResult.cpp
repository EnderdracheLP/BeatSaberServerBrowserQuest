#include "Core/HostedGameData.hpp"
#include "Core/ServerBrowserResult.hpp"

DESERIALIZE_METHOD(ServerBrowser::Core, ServerBrowserResult,
    DESERIALIZE_VALUE(count, Count, Int)
    DESERIALIZE_VALUE(offset, Offset, Int)
    DESERIALIZE_VALUE(limit, Limit, Int)
    DESERIALIZE_VECTOR(lobbies, Lobbies, ServerBrowser::Core::HostedGameData)
    DESERIALIZE_VALUE(message, Message, String)
)