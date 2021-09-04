#pragma once
#include "GlobalNamespace/MultiplayerSessionManager.hpp"
#include "GlobalNamespace/DisconnectedReason.hpp"

namespace ServerBrowser::Game {
    class MpSession {
        /// <summary>
        /// Instance of the game's MultiplayerSessionManager.
        /// </summary>
        static GlobalNamespace::MultiplayerSessionManager* SessionManager; /*{get; private set;}*/

        /// <summary>
        /// Indicates whether we are currently connected to a multiplayer session or not.
        /// </summary>
        static bool IsConnected; /*{get; private set;} = false;*/

        /// <summary>
        /// The most recent reason we were disconnected from a multiplayer session.
        /// </summary>
        static GlobalNamespace::DisconnectedReason DisconnectedReason; /*{get; private set;} = DisconnectedReason.Unknown;*/
    public:
        static GlobalNamespace::MultiplayerSessionManager* get_SessionManager();
        static bool get_IsConnected();
        static GlobalNamespace::DisconnectedReason get_DisconnectedReason();
    };
}