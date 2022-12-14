#include "Game/MpConnect.hpp"
#include "Core/HostedGameData.hpp"

#define RAPIDJSON_HAS_STDSTRING 1

using GlobalNamespace::BeatmapDifficulty;

namespace ServerBrowser::Core {
#pragma region Helpers
    bool HostedGameData::IsOnCustomMaster() {
        return MasterServerHost.has_value() && !MasterServerHost->ends_with(OFFICIAL_MASTER_SUFFIX);
    }
    bool HostedGameData::IsDedicatedServer() {
		if (ServerType.has_value()) {
			return ServerType.value() == ServerTypeBeatDediCustom || ServerType.value() == ServerTypeBeatDediQuickplay || ServerType.value() == ServerTypeVanillaQuickplay;
		}
		else return false;
    }
	bool HostedGameData::IsBeatDedi() {
		if (ServerType.has_value()) {
			return ServerType.value() == ServerTypeBeatDediCustom || ServerType.value() == ServerTypeBeatDediQuickplay;
		}
		else return false;
	}
	bool HostedGameData::SupportsDirectConnect() {
		return IsBeatDedi();
	}
    bool HostedGameData::IsQuickPlayServer() {
		if (ServerType.has_value()) {
			return ServerType.value() == ServerTypeBeatDediQuickplay || ServerType.value() == ServerTypeVanillaQuickplay;
		}
		else return false;
    }
    bool HostedGameData::CanJoin() {
        return !ServerCode.empty() || !HostSecret.has_value() && !HostSecret->empty();
    }
#pragma endregion
#pragma region Describe

	StringW HostedGameData::Describe() {
		std::string moddedDescr = IsModded ? "Modded" : "Vanilla";

		if (IsOnCustomMaster()) {
			moddedDescr += ", Unofficial";
		}

		if (IsQuickPlayServer()) {
			return StringW(string_format("Quick Play Lobby (%d/%d, %s)", PlayerCount, PlayerLimit, moddedDescr.c_str()));
		} else {
			return StringW(GameName + string_format(" (%d/%d, %s)", PlayerCount, PlayerLimit, moddedDescr.c_str()));
		}
	}

	StringW HostedGameData::DescribeType() {
		std::string masterServerDescr;
		std::string moddedDescr;

		if (MasterServerHost.has_value() && MasterServerHost.value() == OFFICIAL_MASTER_OCULUS) {
			masterServerDescr = "Oculus";
		}
		// else if (MasterServerHost.has_value() && MasterServerHost.value() == OFFICIAL_MASTER_STEAM) {
		// 	masterServerDescr = "Steam";
		// }
		else if (MasterServerHost->ends_with(OFFICIAL_MASTER_SUFFIX)) {
			masterServerDescr = "Official-unknown";
		}
		else {
			masterServerDescr = "Unofficial";
		}

		if (IsModded) {
			moddedDescr = "Modded";

			if (MpExVersion.has_value()) {
				moddedDescr += " " + MpExVersion.value();
			}
		}
		else {
			moddedDescr = "Vanilla";
		}
		return StringW(string_format("%s, %s", masterServerDescr.c_str(), moddedDescr.c_str()));
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
					colorHex = "#bcbdc2";
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
	SERIALIZE_VALUE(Id, Id)
	SERIALIZE_VALUE(ServerCode, ServerCode)
	SERIALIZE_VALUE(GameName, GameName)
	//SERIALIZE_VALUE(OwnerName, OwnerName)
	if (!OwnerId.empty()) {
		doc.AddMember("ownerId", OwnerId, alloc);
	}
	SERIALIZE_VALUE(PlayerCount, playerCount)
	SERIALIZE_VALUE(PlayerLimit, playerLimit)
	SERIALIZE_VALUE(IsModded, isModded)
	//SERIALIZE_VALUE(FirstSeen, firstSeen)
	//SERIALIZE_VALUE(LastUpdate, lastUpdate)
	SERIALIZE_VALUE(LobbyState, LobbyState)
	// SERIALIZE_VALUE_OPTIONAL(LevelId, LevelId)
	// SERIALIZE_VALUE_OPTIONAL(SongName, SongName)
	// SERIALIZE_VALUE_OPTIONAL(SongAuthor, SongAuthor)
	SERIALIZE_VALUE_OPTIONAL(Difficulty, Difficulty)
	SERIALIZE_VALUE(Platform, Platform)
	SERIALIZE_VALUE_OPTIONAL(MasterServerHost, MasterServerHost)
	SERIALIZE_VALUE(MasterServerPort, MasterServerPort)
	SERIALIZE_VALUE_OPTIONAL(MasterStatusUrl, MasterStatusUrl)
	// SERIALIZE_VALUE_OPTIONAL(CoverUrl, CoverUrl)
	if (!Players.empty()) {
		rapidjson::Value players(rapidjson::kArrayType);
		for (HostedGamePlayer player : Players) {
			rapidjson::Value playerV(rapidjson::kObjectType);
			playerV.AddMember("SortIndex", player.SortIndex, alloc);
			playerV.AddMember("UserId", player.UserId, alloc);
			playerV.AddMember("UserName", player.UserName, alloc);
			playerV.AddMember("IsHost", player.IsHost, alloc);
			playerV.AddMember("Latency", player.Latency, alloc);
			players.PushBack(playerV, alloc);
		}
		doc.AddMember("Players", players, alloc);
	}
	else doc.AddMember("Players", rapidjson::Value(rapidjson::kNullType), alloc);
	//SERIALIZE_VALUE_OPTIONAL(EndedAt, endedAt)
	//SERIALIZE_VALUE_OPTIONAL(MpExVersion, mpExVersion)
	SERIALIZE_VALUE_OPTIONAL(MpExVersion, mpExVersion)
	SERIALIZE_VALUE_OPTIONAL(MpCoreVersion, mpCoreVersion)
	// if (MpExVersion.has_value())
	// 	doc.AddMember("MpExVersion", MpExVersion->to_string(), alloc);
	// else doc.AddMember("MpExVersion", rapidjson::Value(rapidjson::kNullType), alloc);
	// if (MpCoreVersion.has_value())
	// 	doc.AddMember("MpCoreVersion", MpCoreVersion->to_string(), alloc);
	// else doc.AddMember("MpCoreVersion", rapidjson::Value(rapidjson::kNullType), alloc);
	//SERIALIZE_VALUE(ModName, modName)
	//rapidjson::Value modVer(rapidjson::kObjectType);
	//modVer.PushBack(ModVersion.major, alloc).PushBack(ModVersion.minor, alloc).PushBack(ModVersion.patch, alloc);
	//if (ModVersion.prerelease_number > 0) modVer.PushBack(ModVersion.prerelease_number, alloc);
	//doc.AddMember("ModVersion", modVer, alloc);
	//rapidjson::Value gameVer(rapidjson::kObjectType);
	//gameVer.PushBack(GameVersion.major, alloc).PushBack(GameVersion.minor, alloc).PushBack(GameVersion.patch, alloc);
	//if (GameVersion.prerelease_number > 0) gameVer.PushBack(GameVersion.prerelease_number, alloc);
	//doc.AddMember("gameVersion", gameVer, alloc);
	SERIALIZE_VALUE_OPTIONAL(ServerType, ServerType)
	SERIALIZE_VALUE_OPTIONAL(HostSecret, HostSecret)
	SERIALIZE_VALUE_OPTIONAL(Endpoint, Endpoint)
	SERIALIZE_VALUE_OPTIONAL(ManagerId, ManagerId)
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
			if (jsonValue.HasMember("ownerId") && jsonValue["ownerId"].IsString()) {
				OwnerId = jsonValue["ownerId"].GetString();
			}
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
			DESERIALIZE_VALUE_OPTIONAL(MasterServerHost, masterServerHost, String)
			DESERIALIZE_VALUE(MasterServerPort, masterServerPort, Int)
			DESERIALIZE_VALUE_OPTIONAL(MasterStatusUrl, masterStatusUrl, String)
			DESERIALIZE_VALUE_OPTIONAL(EndedAt, endedAt, String)
			DESERIALIZE_VALUE_OPTIONAL(MpExVersion, mpExVersion, String)
			DESERIALIZE_VALUE_OPTIONAL(MpCoreVersion, mpCoreVersion, String)
			// if (jsonValue.HasMember("mpExVersion") && jsonValue["mpExVersion"].IsString()) {
			// 	MpExVersion = semver::from_string_noexcept(jsonValue["mpExVersion"].GetString());
			// }
			// if (jsonValue.HasMember("mpCoreVersion") && jsonValue["mpCoreVersion"].IsString()) {
			// 	MpCoreVersion = semver::from_string_noexcept(jsonValue["mpCoreVersion"].GetString());
			// }
			DESERIALIZE_VALUE(ModName, modName, String)
			if (jsonValue.HasMember("modVersion") && jsonValue["modVersion"].IsObject()) {
				auto modVer = jsonValue["modVersion"].GetObject();
				ModVersion = string_format("%d.%d.%d", modVer["major"].GetInt(), modVer["minor"].GetInt(), modVer["build"].GetInt());
				if (modVer.HasMember("revision") && modVer["revision"].IsInt())
					ModVersion += string_format("-%d", modVer["revision"].GetInt());
				// ModVersion.major = modVer["major"].GetInt();
				// ModVersion.minor = modVer["minor"].GetInt();
				// ModVersion.patch = modVer["build"].GetInt();
				// if (modVer.HasMember("revision") && modVer["revision"].IsInt())
				// 	ModVersion.prerelease_number = modVer["revision"].GetInt();
			}
			if (jsonValue.HasMember("gameVersion") && jsonValue["gameVersion"].IsObject()) {
				auto gameVer = jsonValue["gameVersion"].GetObject();
				GameVersion = string_format("%d.%d.%d", gameVer["major"].GetInt(), gameVer["minor"].GetInt(), gameVer["build"].GetInt());
				if (gameVer.HasMember("revision") && gameVer["revision"].IsInt())
					GameVersion += string_format("-%d", gameVer["revision"].GetInt());
				// GameVersion.major = gameVer["major"].GetInt();
				// GameVersion.minor = gameVer["minor"].GetInt();
				// GameVersion.patch = gameVer["build"].GetInt();
				// if (gameVer.HasMember("revision") && gameVer["revision"].IsInt())
				// 	GameVersion.prerelease_number = gameVer["revision"].GetInt();
			}
			//DESERIALIZE_CLASS(ModVersion, modVersion)
			//DESERIALIZE_CLASS(GameVersion, gameVersion)
			DESERIALIZE_VALUE_OPTIONAL(ServerType, serverType, String)
			DESERIALIZE_VALUE_OPTIONAL(HostSecret, hostSecret, String)
			DESERIALIZE_VALUE_OPTIONAL(Endpoint, endpoint, String)
			// DESERIALIZE_VALUE_OPTIONAL(ManagerId, managerId, String)

			// TODO: Deserialize level data


	)
#pragma endregion
#pragma endregion