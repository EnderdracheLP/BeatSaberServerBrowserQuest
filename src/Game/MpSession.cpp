#include "Game/MpSession.hpp"
#include "Game/MpConnect.hpp"
#include "Game/MpLocalPlayer.hpp"
#include "PluginConfig.hpp"
#include "multiplayer-core/shared/Utilities.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "System/Action_1.hpp"
#include "System/Action.hpp"
#include "main.hpp"
using GlobalNamespace::IConnectedPlayer;

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

    GlobalNamespace::DisconnectedReason MpSession::get_DisconnectedReason() {
        return DisconnectedReason;
    }

    #pragma region Lifecycle

    System::Action* onSessionJoinDelegate;
    System::Action_1<GlobalNamespace::DisconnectedReason>* onSessionDisconnectDelegate;
    System::Action_1<GlobalNamespace::IConnectedPlayer*>* onPlayerConnectDelegate;
    System::Action_1<GlobalNamespace::IConnectedPlayer*>* onPlayerDisconnectDelegate;

    void MpSession::SetUp() 
    {
        /**
         * Note: The game creates one session manager in MainSystemInit.InstallBindings(), so
         *   we have only one instance that we can use continuously, it won't change.
         */
        SessionManager = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MultiplayerSessionManager*>()->get(0);

        if (SessionManager == nullptr) {
            getLogger().critical("Unable to get MultiplayerSessionManager! Things are broken now.");
            return;
        }
        onSessionJoinDelegate = il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*), 
                            static_cast<Il2CppObject*>(nullptr), OnSessionConnected);
        SessionManager->add_connectedEvent(onSessionJoinDelegate);

        onSessionDisconnectDelegate = il2cpp_utils::MakeDelegate<System::Action_1<GlobalNamespace::DisconnectedReason>*>(
            classof(System::Action_1<GlobalNamespace::DisconnectedReason>*), 
            static_cast<Il2CppObject*>(nullptr), OnSessionDisconnected);
        SessionManager->add_disconnectedEvent(onSessionDisconnectDelegate);

        onPlayerConnectDelegate = il2cpp_utils::MakeDelegate<System::Action_1<IConnectedPlayer*>*>(
            classof(System::Action_1<IConnectedPlayer*>*),
            static_cast<Il2CppObject*>(nullptr), OnSessionPlayerConnected);
        SessionManager->add_playerConnectedEvent(onPlayerConnectDelegate);

        onPlayerDisconnectDelegate = il2cpp_utils::MakeDelegate<System::Action_1<IConnectedPlayer*>*>(
            classof(System::Action_1<IConnectedPlayer*>*), 
            static_cast<Il2CppObject*>(nullptr), OnSessionPlayerDisconnected);
        SessionManager->add_playerDisconnectedEvent(onPlayerDisconnectDelegate);
    }

    void MpSession::TearDown()
    {
        MultiplayerCore::Utilities::ClearDelegate(onSessionJoinDelegate);
        MultiplayerCore::Utilities::ClearDelegate(onSessionDisconnectDelegate);
        MultiplayerCore::Utilities::ClearDelegate(onPlayerConnectDelegate);
        MultiplayerCore::Utilities::ClearDelegate(onPlayerDisconnectDelegate);
        // if (SessionManager != nullptr)
        // {
            // SessionManager->remove_connectedEvent(
            //     il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*),
            //         static_cast<Il2CppObject*>(nullptr), OnSessionConnected));
            // SessionManager->remove_disconnectedEvent(
            //     il2cpp_utils::MakeDelegate<System::Action_1<GlobalNamespace::DisconnectedReason>*>(
            //         classof(System::Action_1<GlobalNamespace::DisconnectedReason>*),
            //         static_cast<Il2CppObject*>(nullptr), OnSessionDisconnected));
            // SessionManager->remove_playerConnectedEvent(
            //     il2cpp_utils::MakeDelegate<System::Action_1<IConnectedPlayer*>*>(classof(System::Action_1<IConnectedPlayer*>*),
            //         static_cast<Il2CppObject*>(nullptr), OnSessionPlayerConnected));
            // SessionManager->remove_playerDisconnectedEvent(
            //     il2cpp_utils::MakeDelegate<System::Action_1<IConnectedPlayer*>*>(classof(System::Action_1<IConnectedPlayer*>*),
            //         static_cast<Il2CppObject*>(nullptr), OnSessionPlayerDisconnected));
        // }
    }
    #pragma endregion

    #pragma region Session events
    void MpSession::OnSessionConnected() {
        getLogger().info("Multiplayer session is now connected.");
        IsConnected = true;
        DisconnectedReason = GlobalNamespace::DisconnectedReason::Unknown;

        // Raise internal event
        //MpEvents::RaiseSessionConnected(SessionManager, new SessionConnectedEventArgs()
        //    {
        //        ConnectionOwner = SessionManager.connectionOwner,
        //        LocalPlayer = SessionManager.localPlayer,
        //        MaxPlayers = SessionManager.maxPlayerCount
        //    });
    }
    void MpSession::OnSessionDisconnected(GlobalNamespace::DisconnectedReason reason) {
        //std::string reasonStr = to_utf8(csstrtostr(System::Enum::GetName(csTypeOf(GlobalNamespace::DisconnectedReason), System::Enum::ToObject(csTypeOf(GlobalNamespace::DisconnectedReason), reason.value))));
        getLogger().info("Multiplayer session is now disconnected (%d).", reason.value /*reasonStr.c_str()*/);

        IsConnected = false;
        DisconnectedReason = reason;

        // Restore the user's preferred master server
        MpConnect::ClearMasterServerOverride();

        // Clear any notifications
        //FloatingNotification.Instance.DismissMessage();

        // Raise internal event
        //MpEvents.RaiseSessionDisconnected(SessionManager, reason);
    }
    void MpSession::OnSessionPlayerConnected(GlobalNamespace::IConnectedPlayer* player) {
        getLogger().info("A player joined the session: %s", to_utf8(csstrtostr(player->get_userName())).c_str());

        // Notification if enabled (and fully connected, because all players raise this on connect)
        //auto isFullyConnected = IsConnected && MpLobbyConnectionTypePatch::ConnectionType != GlobalNamespace::LobbyConnectionType::None;

        //if (isFullyConnected && getPluginConfig().JoinNotificationsEnabled.GetValue())
        //{
        //    FloatingNotification::Instance::ShowMessage(
        //        string_format(
        //        "%d joined!", to_utf8(csstrtostr(player->get_userName())).c_str()),
        //        string_format(
        //        "%d players connected", GetPlayerCount()),
        //        FloatingNotification::NotificationStyle::Blue,
        //        ServerBrowser::Assets::Sprites::get_PortalUser()
        //    );
        //}

        // Raise internal event
        //MpEvents::RaisePlayerConnected(SessionManager, player);
    }
    void MpSession::OnSessionPlayerDisconnected(GlobalNamespace::IConnectedPlayer* player) {
        getLogger().info("A player left the session: %s", to_utf8(csstrtostr(player->get_userName())).c_str());

        // Notification if enabled (and connected, because all players raise this on disconnect)
        //auto isFullyConnected = IsConnected && MpLobbyConnectionTypePatch.ConnectionType != GlobalNamespace::LobbyConnectionType::None;

        //if (isFullyConnected && getPluginConfig().JoinNotificationsEnabled.GetValue())
        //{
        //    auto playerCount = GetPlayerCount();

        //    FloatingNotification.Instance.ShowMessage(
        //        string_format("%s disconnected", to_utf8(csstrtostr(player->get_userName())).c_str()),
        //        playerCount > 1 ? string_format("%d players remaining", playerCount) : "You're all alone",
        //        FloatingNotification::NotificationStyle::Red,
        //        ServerBrowser::Assets::Sprites::get_PortalUser()
        //    );
        //}

        // Raise internal event
        //MpEvents::RaisePlayerDisconnected(SessionManager, player);
    }
    #pragma endregion

    #pragma region Data helpers
    bool MpSession::GetLocalPlayerHasMultiplayerExtensions() {
        return SessionManager->LocalPlayerHasState(il2cpp_utils::newcsstr("modded")) /*|| ModCheck::MultiplayerExtensions::InstalledAndEnabled*/;
    }
    int MpSession::GetPlayerCount() {
        if (!IsConnected)
            return 0;

        // NB: +1 because the local player doesn't count as a "connected player"
        return SessionManager->get_connectedPlayerCount() + 1;
    }
    int MpSession::GetPlayerLimit() {
        //switch (MpLobbyConnectionTypePatch::ConnectionType)
        //{
        //case GlobalNamespace::LobbyConnectionType::PartyHost:
        //    // Custom lobby with its own player limit
        //    return SessionManager->get_maxPlayerCount();
        //default:
            // maxPlayerCount isn't getting set for clients, only hosts, assume 5
            return 5;
        //}
    }
    std::string MpSession::GetHostGameName() {
        //if (MpLobbyConnectionTypePatch::ConnectionType == GlobalNamespace::LobbyConnectionType::QuickPlay)
        //{
        //    return "Quick Play Lobby";
        //}

        std::string finalGameName = "";

        if (MpLocalPlayer::get_UserInfo() != nullptr)
        {
            finalGameName = string_format("%s's game", to_utf8(csstrtostr(MpLocalPlayer::get_UserInfo()->userName)).c_str());
        }
        else getLogger().debug("UserInfo is null");

        if (!getPluginConfig().CustomGameName.GetValue().empty())
        {
            finalGameName = getPluginConfig().CustomGameName.GetValue();
        }
        else getLogger().debug("CustomGameName is empty");

        return !finalGameName.empty() ? finalGameName : "A game";
    }
    std::string MpSession::GetDefaultHostGameName() {
        std::string finalGameName = "";

        if (MpLocalPlayer::get_UserInfo() != nullptr)
        {
            finalGameName = string_format("%s's game", to_utf8(csstrtostr(MpLocalPlayer::get_UserInfo()->userName)).c_str());
        }
        else getLogger().debug("UserInfo is null");

        return !finalGameName.empty() ? finalGameName : "A game";
    }
    #pragma endregion

}