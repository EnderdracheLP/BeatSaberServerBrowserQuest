#include "Game/MpModeSelection.hpp"
#include "Game/MpConnect.hpp"
//#include "Game/MpLobbyDestination.hpp"
#include "GlobalNamespace/SelectMultiplayerLobbyDestination.hpp"
#include "UI/PluginUi.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Object.hpp"
#include "System/Action_1.hpp"
using namespace GlobalNamespace;
using namespace HMUI;
using namespace System;
using namespace ServerBrowser::UI;
using ServerBrowser::Core::HostedGameData;
using ServerBrowser::Game::MpConnect;
using System::Threading::CancellationTokenSource;

namespace ServerBrowser::Game {
	//bool MpModeSelection::WeInitiatedConnection = false;
	//bool MpModeSelection::WeAbortedJoin = false;
	//HostedGameData MpModeSelection::LastConnectToHostedGame;
	// Remove the below for next update
	Il2CppString* MpModeSelection::InjectQuickPlaySecret = nullptr;
	Il2CppString* MpModeSelection::InjectServerCode = nullptr;


	GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* MpModeSelection::_flowCoordinator;
	GlobalNamespace::MultiplayerLobbyConnectionController* MpModeSelection::_mpLobbyConnectionController;
	GlobalNamespace::JoiningLobbyViewController* MpModeSelection::_joiningLobbyViewController;
	GlobalNamespace::SimpleDialogPromptViewController* MpModeSelection::_simpleDialogPromptViewController;


#pragma region Init
	void MpModeSelection::SetUp()
	{
		getLogger().debug("MpModeSelection::SetUp get MultiplayerModeSelectionFlowCoordinator");
		_flowCoordinator = UnityEngine::Object::FindObjectOfType<MultiplayerModeSelectionFlowCoordinator*>();
		getLogger().debug("MpModeSelection::SetUp get multiplayerLobbyConnectionController");
		_mpLobbyConnectionController = _flowCoordinator->multiplayerLobbyConnectionController;
		getLogger().debug("MpModeSelection::SetUp get joiningLobbyViewController");
		_joiningLobbyViewController = _flowCoordinator->joiningLobbyViewController;
		getLogger().debug("MpModeSelection::SetUp get simpleDialogPromptViewController");
		_simpleDialogPromptViewController = _flowCoordinator->simpleDialogPromptViewController;
	}

#pragma endregion

#pragma region Private method helpers
	void MpModeSelection::PresentViewController(ViewController* viewController, ViewController::AnimationDirection animationDirection, bool immediately, Action* finishedCallback) {
		_flowCoordinator->PresentViewController(viewController, finishedCallback, animationDirection, immediately);
	}
	void MpModeSelection::ReplaceTopViewController(ViewController* viewController, ViewController::AnimationType animationType, ViewController::AnimationDirection animationDirection, Action* finishedCallback) {
		_flowCoordinator->ReplaceTopViewController(viewController, finishedCallback, animationType, animationDirection);
	}
	void MpModeSelection::DismissViewController(ViewController* viewController, ViewController::AnimationDirection animationDirection, Action* finishedCallback, bool immediately) {
		_flowCoordinator->DismissViewController(viewController, animationDirection, finishedCallback, immediately);
	}
	void MpModeSelection::SetTitle(std::string title) {
		Il2CppString* titleStr = il2cpp_utils::newcsstr(title);
		_flowCoordinator->SetTitle(titleStr, ViewController::AnimationType::In);
	}
	void MpModeSelection::SetTitle(Il2CppString* title) {
		_flowCoordinator->SetTitle(title, ViewController::AnimationType::In);
	}
#pragma endregion

	void MpModeSelection::OpenCreateServerMenu() {
		// TODO: Make sure any overrides are cleared when we're going to host
		//MpConnect::ClearMasterServerOverride();

		// TODO: If we are initiating the server menu from our UI, assume the intent is to host a game
		//Plugin.Config.LobbyAnnounceToggle = true;
		//Plugin.Config.ShareQuickPlayGames = true;

		_flowCoordinator->HandleMultiplayerLobbyControllerDidFinish(nullptr, MultiplayerModeSelectionViewController::MenuButton::CreateServer);
	}

	void MpModeSelection::ConnectToHostedGame(std::optional<HostedGameData> game) {
		if (!game.has_value()) return;

		GlobalModState::Reset();
		GlobalModState::WeInitiatedConnection = true;
		GlobalModState::LastConnectToHostedGame = game;

		getLogger().info("--> Connecting to lobby destination now"
			" (ServerCode=%s, HostSecret=%s,"
			" ServerType=%s, ServerBrowserId=%d)", 
			game->get_ServerCode().c_str(),
			game->get_HostSecret()->c_str(),
			game->get_ServerType()->c_str(),
			game->get_Id());
		//Plugin.Log.Info("--> Connecting to lobby destination now" +
		//	$" (ServerCode={game->ServerCode}, HostSecret={game->HostSecret}," +
		//	$" ServerType={game->ServerType}, ServerBrowserId={game->Id})");

		_flowCoordinator->joiningLobbyCancellationTokenSource = THROW_UNLESS(il2cpp_utils::New<CancellationTokenSource*>());

		SelectMultiplayerLobbyDestination* MpLobbyDest = THROW_UNLESS(il2cpp_utils::New<SelectMultiplayerLobbyDestination*>(il2cpp_utils::newcsstr(game->get_ServerCode())));
		if (game->get_HostSecret().has_value())
			MpLobbyDest->lobbySecret = il2cpp_utils::newcsstr(game->get_HostSecret().value());

		//SelectMultiplayerLobbyDestination* Destination;
  //      if (game->get_HostSecret().has_value())
		//	Destination = CRASH_UNLESS(il2cpp_utils::New<SelectMultiplayerLobbyDestination*>(il2cpp_utils::newcsstr(game->get_HostSecret().value())));
		//else if (ServerCode)
		//	Destination = CRASH_UNLESS(il2cpp_utils::New<SelectMultiplayerLobbyDestination*>(ServerCode));


		_mpLobbyConnectionController->CreateOrConnectToDestinationParty(
			MpLobbyDest
			//MpLobbyDest
			//il2Cpp_utils::New(MpLobbyDestination(ServerCode, game->HostSecret))
		);

		Il2CppString* LVC_InitStr = il2cpp_utils::newcsstr(string_format("%s (%s)", game->get_GameName().c_str(), game->get_ServerCode().c_str()));
		//_joiningLobbyViewController->Init($"{game->GameName} ({game->ServerCode})");
		_joiningLobbyViewController->Init(LVC_InitStr);


		ReplaceTopViewController(_joiningLobbyViewController, 
			HMUI::ViewController::AnimationType::In,
			HMUI::ViewController::AnimationDirection::Vertical);
	}

	//void MpModeSelection::ConnectToHostedGame(HostedGameData game)
	//{
	//	//if (!game)
	//	//	return;

	//	MpModeSelection::WeInitiatedConnection = true;
	//	MpModeSelection::LastConnectToHostedGame = game;
	//	MpModeSelection::InjectQuickPlaySecret = nullptr;
	//	MpModeSelection::InjectServerCode = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(game->ServerCode);

	//	if (game->IsQuickPlayServer() && !Il2CppString::IsNullOrEmpty(game->HostSecret))
	//	{
	//		// Join Quick Play server by secret
	//		getLogger().info("Trying to join Quick Play game by secret (%s)...", to_utf8(csstrtostr(game->HostSecret)).c_str());

	//		MpModeSelection::InjectQuickPlaySecret = game->HostSecret; // NB: JoinMatchmakingPatch will inject secret

	//		BeatmapDifficultyMask difficultyMask = BeatmapDifficultyMask::All;
	//		//if (game->ServerType == HostedGameData.ServerTypeVanillaQuickplay && game->Difficulty.value != 0) {
	//			// Vanilla quick play is locked to a specific difficulty in the lobby
	//			//difficultyMask = game->Difficulty;
	//		// }

	//		_mpLobbyConnectionController->ConnectToMatchmaking(difficultyMask, SongPackMask::get_all(), true);
	//		_joiningLobbyViewController->Init(il2cpp_utils::newcsstr(string_format("Trying to join %s...", game->GameName.c_str())));

	//		ReplaceTopViewController(_joiningLobbyViewController,
	//			ViewController::AnimationType::In,
	//			ViewController::AnimationDirection::Vertical);
	//	}
	//	else if (!game->ServerCode.empty())
	//	{
	//		// Join party by code
	//		getLogger().info("Trying to join custom game by code (%s)...", game->ServerCode.c_str());

	//		_mpLobbyConnectionController->CreateOrConnectToDestinationParty(il2cpp_utils::newcsstr(game->ServerCode));

	//		_joiningLobbyViewController->Init(il2cpp_utils::newcsstr(string_format("%s (%s)", game->GameName.c_str(), game->ServerCode.c_str())));
	//		ReplaceTopViewController(_joiningLobbyViewController, ViewController::AnimationType::In, ViewController::AnimationDirection::Vertical);
	//	}
	//}

	void MpModeSelection::PresentConnectionFailedError(std::string errorTitle, std::string errorMessage, bool canRetry) {
		CancelLobbyJoin();

		//if (LastConnectToHostedGame == nullptr)
		if (!GlobalModState::LastConnectToHostedGame.has_value())
			canRetry = false; // we don't have game info to retry with
		Il2CppString* errorTitleStr = il2cpp_utils::newcsstr(errorTitle);
		Il2CppString* errorMessageStr = il2cpp_utils::newcsstr(errorMessage);
		Il2CppString* backToBrowserStr = il2cpp_utils::newcsstr("Back to browser");
		Il2CppString* canRetryStr = il2cpp_utils::newcsstr("Retry connection");
		auto* action = il2cpp_utils::MakeDelegate<System::Action_1<int>*>(classof(System::Action_1<int>*), (std::function<void(int)>)[](int btnId) {
				switch (btnId)
				{
				default:
				case 0: // Back to browser
					MpModeSelection::MakeServerBrowserTopView();
					break;
				case 1: // Retry connection
					MpModeSelection::ConnectToHostedGame(GlobalModState::LastConnectToHostedGame);
					break;
				}
			}
		);

		//auto* action = il2cpp_utils::MakeDelegate<System::Action_1<int>*>(classof(System::Action_1<int>*), std::function<void(int)>(ButtonCallback));
		_simpleDialogPromptViewController->Init(errorTitleStr, errorMessageStr, backToBrowserStr, canRetry ? canRetryStr : nullptr, action);
		ReplaceTopViewController(_simpleDialogPromptViewController, ViewController::AnimationType::In, ViewController::AnimationDirection::Vertical);
	}
	void MpModeSelection::CancelLobbyJoin(bool hideLoading) {
            _mpLobbyConnectionController->LeaveLobby();
            
            if (hideLoading)
                _joiningLobbyViewController->HideLoading();
	}
	void MpModeSelection::MakeServerBrowserTopView() {
            ReplaceTopViewController(PluginUi::get_ServerBrowserViewController(), ViewController::AnimationType::In, ViewController::AnimationDirection::Vertical);
	}
}