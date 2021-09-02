#include "Core/GlobalModState.hpp"

namespace ServerBrowser::Core {
    bool GlobalModState::WeInitiatedConnection = false;
    bool GlobalModState::WeAbortedJoin = false;
    std::optional<HostedGameData> GlobalModState::LastConnectToHostedGame = std::nullopt;
    std::optional<std::string> GlobalModState::DirectConnectTarget = std::nullopt;
    bool GlobalModState::ShouldDisableEncryption = false;

    void GlobalModState::Reset()
    {
        WeInitiatedConnection = false;
        WeAbortedJoin = false;
        LastConnectToHostedGame = std::nullopt;
        DirectConnectTarget = std::nullopt;
        ShouldDisableEncryption = false;
    }

}