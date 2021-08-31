#include "Game/MpConnect.hpp"
#include "Game/MpModeSelection.hpp"
using ServerBrowser::Game::MpModeSelection;

namespace ServerBrowser::Game {

	void MpConnect::Join(HostedGameData game) {
		// MQE Version check
		if (game.get_MpExVersion().has_value()) {
			std::string ourMQE_Version = "Undefined";

			if (ourMQE_Version.empty() || ourMQE_Version != game.get_MpExVersion().value()) {
				std::string ourMQE_VersionStr = (!ourMQE_Version.empty() ? ourMQE_Version : "Not installed");
				std::string theirMpExVersionStr = game.get_MpExVersion().value();

				getLogger().warning("Blocking game join because of MultiQuestensions/MultiplayerExtensions version mismatch ours: %s, theirs: %s", 
					ourMQE_Version.c_str(), theirMpExVersionStr.c_str());

				std::string MQE_Error(string_format(
					"MultiQuestensions/MultiplayerExtensions version difference detected!"
					"Please ensure you and the host are both using the latest version."
					""
					"Your version: %s"
					"Their version: %s",
					ourMQE_VersionStr.c_str(),
					theirMpExVersionStr.c_str()
				));
				
				MpModeSelection::PresentConnectionFailedError(
					"Incompatible game",
					MQE_Error,
					false
				);
				return;
			}
		}

		// Master server switching
		if (game.get_MasterServerHost().empty() || game.get_MasterServerHost().ends_with(OFFICIAL_MASTER_SUFFIX)) {
			// Game is hosted on the player platform's official master server
			//if (_usingModdedServer || !_officialEndPoint) {
			//	 If we normally use a modded server (e.g. because BeatTogether is installed), we need to now force-connect to our official server
			//	 
			//	 //TODO: NOT IMPLEMENTED
			//	 
			//	switch (MpLocalPlayer.Platform)
			//	{
			//	case UserInfo.Platform.Oculus:
			//		SetMasterServerOverride(OFFICIAL_MASTER_OCULUS);
			//		break;
			//	case UserInfo.Platform.PS4:
			//		// lmao
			//		// Yeah lol, as if that would ever be used
			//		SetMasterServerOverride(OFFICIAL_MASTER_PS4);
			//		break;
			//	case UserInfo.Platform.Test:
			//		// hmmm
			//		//   ^ Don't think of anything stupid there
			//		SetMasterServerOverride(OFFICIAL_MASTER_TEST);
			//		break;
			//	default:
			//	case UserInfo.Platform.Steam:
			//	case null:
			//		SetMasterServerOverride(OFFICIAL_MASTER_STEAM);
			//		break;
			//	}
			//}
			//else
			//{
				// Clearing the override should fall back to the correct official server
				//ClearMasterServerOverride();
			//}
		}
		else
		{
			// Game is hosted on a custom master server, we need to override
			//SetMasterServerOverride(game.get_MasterServerHost(), game.get_MasterServerPort() != 0 ? game.get_MasterServerPort() : DEFAULT_MASTER_PORT);
		}

		// Trigger the actual join via server code
		MpModeSelection::ConnectToHostedGame(game);
	}

#pragma region Master Server Management

	//MasterServerEndPoint* MpConnect::get_OverrideEndPoint() {

	//}

	//MasterServerEndPoint* MpConnect::get_LastUsedMasterServer() {

	//}
	//void MpConnect::set_OverrideEndPoint(MasterServerEndPoint* NewOverrideEndPoint) {

	//}
	//void MpConnect::set_LastUsedMasterServer(MasterServerEndPoint* LastUsedEndPoint) {

	//}

	//bool MpConnect::get_ShouldDisableCertificateValidation() {

	//}

	//void MpConnect::ReportCurrentMasterServerValue(MasterServerEndPoint* currentEndPoint) {

	//}

	//void MpConnect::SetMasterServerOverride(std::string hostName, int port) {

	//}

	//void MpConnect::SetMasterServerOverride(MasterServerEndPoint* overrideEndPoint) {

	//}

	//void MpConnect::ClearMasterServerOverride() {

	//}
#pragma endregion
}