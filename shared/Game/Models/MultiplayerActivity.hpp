#include <string>
#include "GlobalNamespace/DnsEndPoint.hpp"
#include "GlobalNamespace/MultiplayerLobbyConnectionController.hpp"
#include "GlobalNamespace/MultiplayerLobbyState.hpp"
#include "GlobalNamespace/BeatmapLevelSelectionMask.hpp"
#include "GlobalNamespace/GameplayServerConfiguration.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"


namespace ServerBrowser::Game::Models {
    struct MultiplayerActivity {
        #pragma region Fields
        bool InOnlineMenu;
        std::string Name;
        SafePtr<GlobalNamespace::DnsEndPoint> MasterServer;
        GlobalNamespace::MultiplayerLobbyConnectionController::LobbyConnectionType ConnectionType;
        GlobalNamespace::MultiplayerLobbyState LobbyState;
        std::string ServerCode;
        std::string HostUserId;
        std::string HostSecret;
        std::string Endpoint;
        bool IsDedicatedServer;
        GlobalNamespace::BeatmapLevelSelectionMask SelectionMask;
        GlobalNamespace::GameplayServerConfiguration ServerConfiguration;
        int MaxPlayerCount;
        std::vector<GlobalNamespace::IConnectedPlayer*> Players;
        SafePtr<GlobalNamespace::IPreviewBeatmapLevel> CurrentLevel;
        GlobalNamespace::BeatmapDifficulty CurrentDifficulty;
        GlobalNamespace::BeatmapCharacteristicSO* CurrentCharacteristic;
        GlobalNamespace::GameplayModifiers* CurrentModifiers;
        int SessionStartedAt;
        std::string ManagerId;
        #pragma endregion
    };
}