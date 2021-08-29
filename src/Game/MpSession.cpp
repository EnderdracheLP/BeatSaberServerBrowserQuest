#include "Game/MpSession.hpp"

namespace ServerBrowser::Game {
    GlobalNamespace::MultiplayerSessionManager* MpSession::SessionManager;
    bool MpSession::IsConnected = false;
    GlobalNamespace::DisconnectedReason MpSession::DisconnectedReason = GlobalNamespace::DisconnectedReason::Unknown;

    void MpSession::set_SessionManager(GlobalNamespace::MultiplayerSessionManager* sessionManager) {
        SessionManager = sessionManager;
    }

    GlobalNamespace::MultiplayerSessionManager* MpSession::get_SessionManager() {
        return SessionManager;
    }

    void MpSession::set_IsConnected(bool isConnected) {
        IsConnected = isConnected;
    }

    bool MpSession::get_IsConnected() {
        return IsConnected;
    }


}