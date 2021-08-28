#include "Game/MpConnect.hpp"
#include "Core/HostedGameData.hpp"

using GlobalNamespace::BeatmapDifficulty;

namespace ServerBrowser::Core {
#pragma region Helpers
    bool HostedGameData::IsOnCustomMaster() {
        return !MasterServerHost.empty() && !MasterServerHost.ends_with(OFFICIAL_MASTER_SUFFIX);
    }
    bool HostedGameData::IsDedicatedServer() {
		if (ServerType.has_value()) {
			return ServerType.value() == ServerTypeBeatDediCustom || ServerType.value() == ServerTypeBeatDediQuickplay || ServerType.value() == ServerTypeVanillaQuickplay;
		}
		else return false;
    }
    bool HostedGameData::IsQuickPlayServer() {
		if (ServerType.has_value()) {
			return ServerType.value() == ServerTypeBeatDediQuickplay || ServerType.value() == ServerTypeVanillaQuickplay;
		}
		else return false;
    }
    bool HostedGameData::CanJoin() {
        return !ServerCode.empty() || !HostSecret.has_value();
    }
#pragma endregion
#pragma region Describe

	Il2CppString* HostedGameData::Describe() {
		std::string moddedDescr = IsModded ? "Modded" : "Vanilla";

		if (IsQuickPlayServer()) {
			moddedDescr += " Quick Play";
		}

		if (IsOnCustomMaster()) {
			moddedDescr += ", Cross-play";
		}
		return il2cpp_utils::newcsstr(GameName + string_format(" (%d/%d, %s)", PlayerCount, PlayerLimit, moddedDescr.c_str()));
	}

	Il2CppString* HostedGameData::DescribeType() {
		std::string masterServerDescr;
		std::string moddedDescr;

		if (MasterServerHost.empty() || MasterServerHost == OFFICIAL_MASTER_OCULUS) {
			masterServerDescr = "Oculus";
		}
		else if (MasterServerHost.ends_with(OFFICIAL_MASTER_SUFFIX)) {
			masterServerDescr = "Official-unknown";
		}
		else {
			masterServerDescr = "Cross-play";
		}

		if (IsModded) {
			moddedDescr = "Modded";

			if (!MpExVersion.empty()) {
				moddedDescr += MpExVersion;
			}
		}
		else {
			moddedDescr = "Vanilla";
		}
		return il2cpp_utils::newcsstr(string_format("%s, %s", masterServerDescr.c_str(), moddedDescr.c_str()));
	}

	std::string HostedGameData::DescribeDifficulty(bool withColorTag) {
		std::string text;
		std::string colorHex;

		if (Difficulty.has_value()) {
			switch (Difficulty.value())
			{
			case BeatmapDifficulty::Easy:
				text = "Easy";
				break;
			case BeatmapDifficulty::Normal:
				text = "Normal";
				break;
			case BeatmapDifficulty::Hard:
				text = "Hard";
				break;
			case BeatmapDifficulty::Expert:
				text = "Expert";
				break;
			case BeatmapDifficulty::ExpertPlus:
				text = "Expert+";
				break;
			default:
				text = "Unknown";
				break;
			}

			if (withColorTag) {
				switch (Difficulty.value())
				{
				case BeatmapDifficulty::Easy:
					colorHex = "#3cb371";
					break;
				case BeatmapDifficulty::Normal:
					colorHex = "#59b0f4";
					break;
				case BeatmapDifficulty::Hard:
					colorHex = "#ff6347";
					break;
				case BeatmapDifficulty::Expert:
					colorHex = "#bf2a42";
					break;
				case BeatmapDifficulty::ExpertPlus:
					colorHex = "#8f48db";
					break;
				default:
					colorHex = "#FFFFFF";
					break;
				}
			text = string_format("<color=%s>%s</color>", colorHex.c_str(), text.c_str());
			}
		}
		else text = "Unknown";
		return text;
	}
}
#pragma endregion
#pragma region JSON Serialization/Deserialization
#pragma region Serialize
SERIALIZE_STRING_METHOD(ServerBrowser::Core, HostedGameData,
	SERIALIZE_VALUE_OPTIONAL(BeatsaverId, beatsaverId)
	SERIALIZE_VALUE_OPTIONAL(CoverUrl, coverUrl)
	SERIALIZE_VALUE_OPTIONAL(LevelName, levelName)
	SERIALIZE_VALUE(Id, id)
	SERIALIZE_VALUE(ServerCode, serverCode)
	SERIALIZE_VALUE(GameName, gameName)
	SERIALIZE_VALUE(OwnerName, ownerName)
	SERIALIZE_VALUE(PlayerCount, playerCount)
	SERIALIZE_VALUE(PlayerLimit, playerLimit)
	SERIALIZE_VALUE(IsModded, isModded)
	SERIALIZE_VALUE(FirstSeen, firstSeen)
	SERIALIZE_VALUE(LastUpdate, lastUpdate)
	SERIALIZE_VALUE(LobbyState, lobbyState)
	SERIALIZE_VALUE_OPTIONAL(LevelId, levelId)
	SERIALIZE_VALUE_OPTIONAL(SongName, songName)
	SERIALIZE_VALUE_OPTIONAL(SongAuthor, songAuthor)
	SERIALIZE_VALUE_OPTIONAL(Difficulty, difficulty)
	SERIALIZE_VALUE(Platform, platform)
	SERIALIZE_VALUE(MasterServerHost, masterServerHost)
	SERIALIZE_VALUE(MasterServerPort, masterServerPort)
	SERIALIZE_VALUE_OPTIONAL(EndedAt, endedAt)
	SERIALIZE_VALUE(MpExVersion, mpExVersion)
	rapidjson::Value modVer(rapidjson::kObjectType);
	modVer.PushBack(ModVersion.GetMajor(), alloc).PushBack(ModVersion.GetMinor(), alloc).PushBack(ModVersion.GetBuild(), alloc);
	if (ModVersion.GetRevision().has_value()) modVer.PushBack(ModVersion.GetRevision().value(), alloc);
	doc.AddMember("modVersion", modVer, alloc);
	rapidjson::Value gameVer(rapidjson::kObjectType);
	gameVer.PushBack(GameVersion.GetMajor(), alloc).PushBack(GameVersion.GetMinor(), alloc).PushBack(GameVersion.GetBuild(), alloc);
	if (GameVersion.GetRevision().has_value()) gameVer.PushBack(GameVersion.GetRevision().value(), alloc);
	doc.AddMember("gameVersion", gameVer, alloc);
	SERIALIZE_VALUE_OPTIONAL(ServerType, serverType)
	SERIALIZE_VALUE_OPTIONAL(HostSecret, hostSecret)
	SERIALIZE_VALUE_OPTIONAL(Endpoint, endpoint)
)
#pragma endregion
#pragma region Deserialize
	DESERIALIZE_METHOD(ServerBrowser::Core, HostedGameData,
			DESERIALIZE_VALUE_OPTIONAL(BeatsaverId, beatsaverId, String)
			DESERIALIZE_VALUE_OPTIONAL(CoverUrl, coverUrl, String)
			DESERIALIZE_VALUE_OPTIONAL(LevelName, levelName, String)
			DESERIALIZE_VALUE(Id, id, Int)
			DESERIALIZE_VALUE(ServerCode, serverCode, String)
			DESERIALIZE_VALUE(GameName, gameName, String)
			DESERIALIZE_VALUE(OwnerName, ownerName, String)
			DESERIALIZE_VALUE(PlayerCount, playerCount, Int)
			DESERIALIZE_VALUE(PlayerLimit, playerLimit, Int)
			DESERIALIZE_VALUE(IsModded, isModded, Bool)
			DESERIALIZE_VALUE(FirstSeen, firstSeen, String)
			DESERIALIZE_VALUE(LastUpdate, lastUpdate, String)
			DESERIALIZE_VALUE(LobbyState, lobbyState, Int)
			DESERIALIZE_VALUE_OPTIONAL(LevelId, levelId, String)
			DESERIALIZE_VALUE_OPTIONAL(SongName, songName, String)
			DESERIALIZE_VALUE_OPTIONAL(SongAuthor, songAuthor, String)
			DESERIALIZE_VALUE_OPTIONAL(Difficulty, difficulty, Int)
			DESERIALIZE_VALUE(Platform, platform, String)
			DESERIALIZE_VALUE(MasterServerHost, masterServerHost, String)
			DESERIALIZE_VALUE(MasterServerPort, masterServerPort, Int)
			DESERIALIZE_VALUE_OPTIONAL(EndedAt, endedAt, String)
			DESERIALIZE_VALUE(MpExVersion, mpExVersion, String)
			DESERIALIZE_CLASS(ModVersion, modVersion)
			DESERIALIZE_CLASS(GameVersion, gameVersion)
			DESERIALIZE_VALUE_OPTIONAL(ServerType, serverType, String)
			DESERIALIZE_VALUE_OPTIONAL(HostSecret, hostSecret, String)
			DESERIALIZE_VALUE_OPTIONAL(Endpoint, endpoint, String)
	)
#pragma endregion
#pragma endregion