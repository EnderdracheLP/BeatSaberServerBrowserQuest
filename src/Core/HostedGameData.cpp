#include "Core/HostedGameData.hpp"
#include "Game/MpConnect.hpp"
using namespace GlobalNamespace;
using ServerBrowser::Game::MpConnect;

namespace ServerBrowser::Core {

#pragma region Helpers
	bool HostedGameData::IsOnCustomMaster() {
		return !MasterServerHost.empty() && !MasterServerHost.ends_with(OFFICIAL_MASTER_SUFFIX);
	}
	bool HostedGameData::IsDedicatedServer() {
		return ServerType == ServerTypeBeatDediCustom || ServerType == ServerTypeBeatDediQuickplay || ServerType == ServerTypeVanillaQuickplay;
	}
	bool HostedGameData::IsQuickPlayServer() {
		return ServerType == ServerTypeBeatDediQuickplay || ServerType == ServerTypeVanillaQuickplay;
	}
	bool HostedGameData::CanJoin() {
		return !ServerCode.empty() || !Il2CppString::IsNullOrEmpty(HostSecret);
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

	Il2CppString* HostedGameData::DescribeDifficulty(bool withColorTag) {
		std::string text;

		switch (Difficulty)
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
			std::string colorHex;
			switch (Difficulty)
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
		return il2cpp_utils::newcsstr(text);
	}
#pragma endregion

#pragma region Serialize
	Il2CppString* ToJson() {
		// Not Implemented
		return nullptr;
	}
}