#pragma once
#include "main.hpp"
#include "GlobalNamespace/MultiplayerLobbyState.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"

namespace ServerBrowser::Core {
	class HostedGameData {
#pragma region Consts
	public:
        const Il2CppString* ServerTypePlayerHost = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("player_host");
        const Il2CppString* ServerTypeBeatDediCustom = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("beatdedi_custom");
        const Il2CppString* ServerTypeBeatDediQuickplay = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("beatdedi_quickplay");
        const Il2CppString* ServerTypeVanillaQuickplay = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("vanilla_quickplay");
#pragma endregion

#pragma region Fields
        int Id;
        //int get_Id();
        //void set_Id(int value);

        std::string ServerCode;
        //Il2CppString* get_ServerCode();
        //void set_ServerCode(Il2CppString* value);

        std::string GameName;
        //Il2CppString* get_GameName();
        //void set_GameName(Il2CppString* value);

        Il2CppString* OwnerId;
        //Il2CppString* get_OwnerId();
        //void set_OwnerId(Il2CppString* value);

        int PlayerCount;
        //int get_PlayerCount();
        //void set_PlayerCount(int value);

        int PlayerLimit;
        //int get_PlayerLimit();
        //void set_PlayerLimit(int value);

        bool IsModded;
        //bool get_IsModded();
        //void set_IsModded(bool value);

        GlobalNamespace::MultiplayerLobbyState LobbyState = GlobalNamespace::MultiplayerLobbyState::None;
        //GlobalNamespace::MultiplayerLobbyState get_LobbyState();
        //void set_LobbyState(GlobalNamespace::MultiplayerLobbyState state);

        Il2CppString* LevelId = nullptr;
        Il2CppString* SongName = nullptr;
        Il2CppString* SongAuhtor = nullptr;
        GlobalNamespace::BeatmapDifficulty Difficulty;
        Il2CppString* Platform = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("unknown");
        std::string MasterServerHost;
        int MasterServerPort;
        //string ? CoverUrl{ get; set; } = null;
        //List<HostedGamePlayer> ? Players{ get; set; } = null;
        //SemVer.Version ? MpExVersion{ get; set; } = null;
        std::string MpExVersion;
        Il2CppString* ServerType = nullptr;
        Il2CppString* HostSecret = nullptr;
#pragma endregion

#pragma region Helpers
        bool IsOnCustomMaster();
        bool IsDedicatedServer();
        bool IsQuickPlayServer();
        bool CanJoin();

#pragma region Describe
        Il2CppString* Describe();
        Il2CppString* DescribeType();
        Il2CppString* DescribeDifficulty(bool withColorTag = false);
#pragma endregion

#pragma region Serialize
        Il2CppString* ToJson();
#pragma endregion

	};
}