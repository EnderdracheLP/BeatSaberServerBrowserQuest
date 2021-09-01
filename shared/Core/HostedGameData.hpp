#pragma once
#include "main.hpp"
#include "GlobalNamespace/MultiplayerLobbyState.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "semver/include/semver.hpp" // SemVer Submodule for version matching
#include <optional>
#include "TypeMacros.hpp"
//#include "SemVer.hpp"

namespace ServerBrowser::Core {
    class HostedGameData {
#pragma region Consts
    private:
        std::string ServerTypePlayerHost = "player_host";
        std::string ServerTypeBeatDediCustom = "beatdedi_custom";
        std::string ServerTypeBeatDediQuickplay = "beatdedi_quickplay";
        std::string ServerTypeVanillaQuickplay = "vanilla_quickplay";
    public:
        const std::string get_ServerTypePlayerHost() const { return ServerTypePlayerHost; }
        const std::string get_ServerTypeBeatDediCustom() const { return ServerTypeBeatDediCustom; }
        const std::string get_ServerTypeBeatDediQuickplay() const { return ServerTypeBeatDediQuickplay; }
        const std::string get_ServerTypeVanillaQuickplay() const { return ServerTypeVanillaQuickplay; }
#pragma endregion
#pragma region Helpers
        bool IsOnCustomMaster();
        bool IsDedicatedServer();
        bool IsQuickPlayServer();
        bool CanJoin();
#pragma endregion

#pragma region Describe
        Il2CppString* Describe();
        Il2CppString* DescribeType();
        std::string DescribeDifficulty(bool withColorTag = false);
#pragma endregion

#pragma region Fields
        void Deserialize(const rapidjson::Value& jsonValue);

    private:
        std::optional<std::string> BeatsaverId;
        std::optional<std::string> CoverUrl;
        std::optional<std::string> LevelName;
        int Id;
        std::string ServerCode;
        std::string GameName;
        std::string OwnerName;
        int PlayerCount;
        int PlayerLimit;
        bool IsModded;
        std::string FirstSeen;
        std::string LastUpdate;
        int LobbyState;
        std::optional<std::string> LevelId;
        std::optional<std::string> SongName;
        std::optional<std::string> SongAuthor;
        std::optional<int> Difficulty;
        std::string Platform;
        std::string MasterServerHost;
        int MasterServerPort;
        std::optional<std::string> EndedAt;
        std::optional<semver::version> MpExVersion;
        semver::version ModVersion;
        semver::version GameVersion;
        //SemVer ModVersion;
        //SemVer GameVersion;
        std::optional<std::string> ServerType;
        std::optional<std::string> HostSecret;
        std::optional<std::string> Endpoint;

    public:
        const std::optional<std::string> get_BeatsaverId() const { return BeatsaverId; }
        const std::optional<std::string> get_CoverUrl() const { return CoverUrl; }
        const std::optional<std::string> get_LevelName() const { return LevelName; }
        const int& get_Id() const { return Id; }
        const std::string& get_ServerCode() const { return ServerCode; }
        const std::string& get_GameName() const { return GameName; }
        const std::string& get_OwnerName() const { return OwnerName; }
        const int& get_PlayerCount() const { return PlayerCount; }
        const int& get_PlayerLimit() const { return PlayerLimit; }
        const bool& get_IsModded() const { return IsModded; }
        const std::string& get_FirstSeen() const { return FirstSeen; }
        const std::string& get_LastUpdate() const { return LastUpdate; }
        const int& get_LobbyState() const { return LobbyState; }
        const std::optional<std::string> get_LevelId() const { return LevelId; }
        const std::optional<std::string> get_SongName() const { return SongName; }
        const std::optional<std::string> get_SongAuthor() const { return SongAuthor; }
        const std::optional<int> get_Difficulty() const { return Difficulty; }
        const std::string& get_Platform() const { return Platform; }
        const std::string& get_MasterServerHost() const { return MasterServerHost; }
        const int& get_MasterServerPort() const { return MasterServerPort; }
        const std::optional<std::string> get_EndedAt() const { return EndedAt; }
        const std::optional<semver::version> get_MpExVersion() const { return MpExVersion; }
        const semver::version& get_ModVersion() const { return ModVersion; }
        const semver::version& get_GameVersion() const { return GameVersion; }
        //const SemVer& get_ModVersion() const { return ModVersion; }
        //const SemVer& get_GameVersion() const { return GameVersion; }
        const std::optional<std::string> get_ServerType() const { return ServerType; }
        const std::optional<std::string> get_HostSecret() const { return HostSecret; }
        const std::optional<std::string> get_Endpoint() const { return Endpoint; }

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

        const std::string SerializeToString();
#pragma endregion
    };
}