#pragma once
#include "main.hpp"
#include "GlobalNamespace/MultiplayerLobbyState.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "cpp-semver/shared/cpp-semver.hpp"
// #include "semver/include/semver.hpp" // SemVer Submodule for version matching
#include <optional>
#include "TypeMacros.hpp"
#include "Core/HostedGamePlayer.hpp"
//#include "SemVer.hpp"

namespace ServerBrowser::Core {
    class HostedGameData {
#pragma region Consts
    private:
        std::string ServerTypeBeatTogetherDedicated = "beattogether_dedicated";
        std::string ServerTypeBeatTogetherQuickplay = "beattogether_quickplay";
        std::string ServerTypeBeatDediCustom = "beatdedi_custom";
        std::string ServerTypeBeatDediQuickplay = "beatdedi_quickplay";
        std::string ServerTypeVanillaDedicated = "vanilla_dedicated";
        std::string ServerTypeVanillaQuickplay = "vanilla_quickplay";
        std::string ServerTypePlayerHost = "player_host";
    public:
        const std::string get_ServerTypePlayerHost() const { return ServerTypePlayerHost; }
        const std::string get_ServerTypeBeatDediCustom() const { return ServerTypeBeatDediCustom; }
        const std::string get_ServerTypeBeatDediQuickplay() const { return ServerTypeBeatDediQuickplay; }
        const std::string get_ServerTypeVanillaQuickplay() const { return ServerTypeVanillaQuickplay; }
#pragma endregion
#pragma region Helpers
        bool IsOnCustomMaster();
        bool IsDedicatedServer();
        bool IsBeatDedi();
        bool SupportsDirectConnect();
        bool IsQuickPlayServer();
        bool CanJoin();
#pragma endregion

#pragma region Describe
        StringW Describe();
        StringW DescribeType();
        std::string DescribeDifficulty(bool withColorTag = false);
#pragma endregion

#pragma region Fields
        void Deserialize(const rapidjson::Value& jsonValue);

    private:
        // TODO: Implement the level class for these and update code
        std::optional<std::string> CoverUrl;
        std::optional<std::string> LevelId;
        std::optional<std::string> SongName;
        std::optional<std::string> SongAuthor;

        std::optional<std::string> BeatsaverId;
        std::optional<std::string> LevelName;
        int Id;
        std::string ServerCode;
        std::string GameName;
        std::string OwnerId;
        std::string OwnerName;
        int PlayerCount;
        int PlayerLimit;
        bool IsModded;
        std::string FirstSeen;
        std::string LastUpdate;
        int LobbyState;
        std::optional<int> Difficulty;
        std::string Platform;
        std::optional<std::string> MasterServerHost;
        int MasterServerPort;
        std::optional<std::string> MasterStatusUrl;
        std::vector<HostedGamePlayer> Players;
        std::optional<std::string> EndedAt;
        std::optional<std::string> MpExVersion;
        std::optional<std::string> MpCoreVersion;
        std::string ModName;
        std::string ModVersion;
        std::string GameVersion;
        //SemVer ModVersion;
        //SemVer GameVersion;
        std::optional<std::string> ServerType;
        std::optional<std::string> HostSecret;
        std::optional<std::string> Endpoint;
        std::optional<std::string> ManagerId;
        std::optional<std::string> Key;

    public:
        // TODO: Implement the level class for these and update code
        const std::optional<std::string> get_CoverUrl() const { return CoverUrl; }
        const std::optional<std::string> get_LevelId() const { return LevelId; }
        const std::optional<std::string> get_SongName() const { return SongName; }
        const std::optional<std::string> get_SongAuthor() const { return SongAuthor; }

        const std::optional<std::string> get_BeatsaverId() const { return BeatsaverId; }
        const std::optional<std::string> get_LevelName() const { return LevelName; }
        const int& get_Id() const { return Id; }
        const std::string& get_ServerCode() const { return ServerCode; }
        const std::string& get_GameName() const { return GameName; }
        const std::string& get_OwnerId() const { return OwnerId; }
        const std::string& get_OwnerName() const { return OwnerName; }
        const int& get_PlayerCount() const { return PlayerCount; }
        const int& get_PlayerLimit() const { return PlayerLimit; }
        const bool& get_IsModded() const { return IsModded; }
        const std::string& get_FirstSeen() const { return FirstSeen; }
        const std::string& get_LastUpdate() const { return LastUpdate; }
        const int& get_LobbyState() const { return LobbyState; }
        const std::optional<int> get_Difficulty() const { return Difficulty; }
        const std::string& get_Platform() const { return Platform; }
        const std::optional<std::string> get_MasterServerHost() const { return MasterServerHost; }
        const int& get_MasterServerPort() const { return MasterServerPort; }
        const std::optional<std::string> get_MasterStatusUrl() const { return MasterStatusUrl; }
        const std::optional<std::string> get_EndedAt() const { return EndedAt; }
        const std::optional<std::string> get_MpExVersion() const { return MpExVersion; }
        const std::optional<std::string> get_MpCoreVersion() const { return MpCoreVersion; }
        const std::string& get_ModName() const { return ModName; }
        const std::string& get_ModVersion() const { return ModVersion; }
        const std::string& get_GameVersion() const { return GameVersion; }
        //const SemVer& get_ModVersion() const { return ModVersion; }
        //const SemVer& get_GameVersion() const { return GameVersion; }
        const std::optional<std::string> get_ServerType() const { return ServerType; }
        const std::optional<std::string> get_HostSecret() const { return HostSecret; }
        const std::optional<std::string> get_Endpoint() const { return Endpoint; }
        const std::optional<std::string> get_ManagerId() const { return ManagerId; }

        //void set_BeatsaverId(std::string value);
        //void set_CoverUrl(std::string value);
        //void set_LevelName(std::string value);
        //void set_Id(int value)
        //void set_ServerCode() const { return ServerCode; }
        //const std::string& get_GameName() const { return GameName; }
        //const std::string& get_OwnerName() const { return OwnerName; }
        //const int& get_PlayerCount() const { return PlayerCount; }
        //const int& get_PlayerLimit() const { return PlayerLimit; }
        //const bool& get_IsModded() const { return IsModded; }
        //const std::string& get_FirstSeen() const { return FirstSeen; }
        //const std::string& get_LastUpdate() const { return LastUpdate; }
        //const int& get_LobbyState() const { return LobbyState; }
        //const std::optional<std::string> get_LevelId() const { return LevelId; }
        //const std::optional<std::string> get_SongName() const { return SongName; }
        //const std::optional<std::string> get_SongAuthor() const { return SongAuthor; }
        //const std::optional<int> get_Difficulty() const { return Difficulty; }
        //const std::string& get_Plattform() const { return Plattform; }
        //const std::string& get_MasterServerHost() const { return MasterServerHost; }
        //const int& get_MasterServerPort() const { return MasterServerPort; }
        //const std::optional<std::string> get_EndedAt() const { return EndedAt; }
        //const std::string& get_MpExVersion() const { return MpExVersion; }
        //const SemVer& get_ModVersion() const { return ModVersion; }
        //const SemVer& get_GameVersion() const { return GameVersion; }
        //const std::optional<std::string> get_ServerType() const { return ServerType; }
        //const std::optional<std::string> get_HostSecret() const { return HostSecret; }
        //const std::optional<std::string> get_Endpoint() const { return Endpoint; }

        const std::string ToJSON();
#pragma endregion
    };
}