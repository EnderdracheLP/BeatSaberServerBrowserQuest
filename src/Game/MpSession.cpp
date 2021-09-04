#include "Game/MpSession.hpp"

namespace ServerBrowser::Game {
    GlobalNamespace::MultiplayerSessionManager* MpSession::SessionManager;
    bool MpSession::IsConnected = false;
    GlobalNamespace::DisconnectedReason MpSession::DisconnectedReason = GlobalNamespace::DisconnectedReason::Unknown;

    GlobalNamespace::MultiplayerSessionManager* MpSession::get_SessionManager() {
        return SessionManager;
    }

    bool MpSession::get_IsConnected() {
        return IsConnected;
    }


}