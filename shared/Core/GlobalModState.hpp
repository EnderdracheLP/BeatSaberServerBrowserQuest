#pragma once
//#include "System/Net/IPEndpoint.hpp"
#include "HostedGameData.hpp"
#include <optional>
#include <string>

namespace ServerBrowser::Core {
    /// <summary>
    /// Global connection modifiers used by Server Browser Hooks.
    /// </summary>
    class GlobalModState {
    public:
        /// <summary>
        /// If true, the server browser initiated the current connection.
        /// This indicates that connection checks and patches may be enabled.
        /// </summary>
        static bool WeInitiatedConnection/* = false*/;

        /// <summary>
        /// If true, the server browser aborted a join it had previously initiated.
        /// This typically happens when a dedicated server instance is no longer available.
        /// This value is used to present the appropriate error message to the user.
        /// </summary>
        static bool WeAbortedJoin/* = false*/;

        /// <summary>
        /// The last game the server browser attempted to connect to.
        /// </summary>
        static std::optional<HostedGameData> LastConnectToHostedGame/* = null*/;

        /// <summary>
        /// If set, performs a direct server connection to the target IPEndPoint.
        /// This will effectively ignore and override what the master server tells the client.
        /// Important: This ONLY works for servers that explicitly support this (BeatDedi). 
        /// </summary>
        static std::optional<std::string> DirectConnectTarget;
        //static IPEndPoint* DirectConnectTarget = null;

        /// <summary>
        /// Indicates whether encryption should be disabled on connections.
        /// This is used to enable direct connections to servers that support it, as there is no encryption handshake.
        /// </summary>
        static bool ShouldDisableEncryption/* = false*/;

        /// <summary>
        /// Resets the mod state, disabling any special networking patches and behaviors.
        /// </summary>
        static void Reset();
    };
}