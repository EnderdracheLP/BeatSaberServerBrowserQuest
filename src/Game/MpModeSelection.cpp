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
	bool MpModeSelection::WeInitiatedConnection = false;
	bool MpModeSelection::WeAbortedJoin = false;
	HostedGameData MpModeSelection::LastConnectToHostedGame;

	GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* MpModeSelection::_flowCoordinator;
	GlobalNamespace::MultiplayerLobbyConnectionController* MpModeSelection::_mpLobbyConnectionController;
	GlobalNamespace::JoiningLobbyViewController* MpModeSelection::_joiningLobbyViewController;
	GlobalNamespace::SimpleDialogPromptViewController* MpModeSelection::_simpleDialogPromptViewController;


#pragma region Init
	void MpModeSelection::SetUp()
	{
		_flowCoordinator = UnityEngine::Object::FindObjectOfType<GlobalNamespace::MultiplayerModeSelectionFlowCoordinator*>();
		_mpLobbyConnectionController = _flowCoordinator->multiplayerLobbyConnectionController;
		_joiningLobbyViewController = _flowCoordinator->joiningLobbyViewController;
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
		// Make sure any overrides are cleared when we're going to host
		MpConnect::ClearMasterServerOverride();

		// TODO: If we are initiating the server menu from our UI, assume the intent is to host a game
		//Plugin.Config.LobbyAnnounceToggle = true;
		//Plugin.Config.ShareQuickPlayGames = true;

		_flowCoordinator->HandleMultiplayerLobbyControllerDidFinish(nullptr, MultiplayerModeSelectionViewController::MenuButton::CreateServer);
	}
	 //TODO: Add ConnectToHostGame
	void MpModeSelection::ConnectToHostedGame(HostedGameData game) {
		//if (!game) return;

		WeInitiatedConnection = true;
		WeAbortedJoin = false;
		LastConnectToHostedGame = game;

		getLogger().info("--> Connecting to lobby destination now"
			" (ServerCode=%s, HostSecret=%s,"
			" ServerType=%s, ServerBrowserId=%d)", 
			game.ServerCode.c_str(),
			to_utf8(csstrtostr(game.HostSecret)).c_str(),
			to_utf8(csstrtostr(game.ServerType)).c_str(),
			game.Id);
		//Plugin.Log.Info("--> Connecting to lobby destination now" +
		//	$" (ServerCode={game.ServerCode}, HostSecret={game.HostSecret}," +
		//	$" ServerType={game.ServerType}, ServerBrowserId={game.Id})");

		_flowCoordinator->joiningLobbyCancellationTokenSource = THROW_UNLESS(il2cpp_utils::New<CancellationTokenSource*>());

		Il2CppString* ServerCode = il2cpp_utils::newcsstr(game.ServerCode);
		SelectMultiplayerLobbyDestination* MpLobbyDest = THROW_UNLESS(il2cpp_utils::New<SelectMultiplayerLobbyDestination*>(ServerCode));
		MpLobbyDest->lobbySecret = game.HostSecret;
		_mpLobbyConnectionController->CreateOrConnectToDestinationParty(
			MpLobbyDest
			//il2Cpp_utils::New(MpLobbyDestination(ServerCode, game.HostSecret))
		);

		Il2CppString* LVC_InitStr = il2cpp_utils::newcsstr(string_format("%s (%s)", game.GameName.c_str(), game.ServerCode.c_str()));
		//_joiningLobbyViewController->Init($"{game.GameName} ({game.ServerCode})");
		_joiningLobbyViewController->Init(LVC_InitStr);


		ReplaceTopViewController(_joiningLobbyViewController, 
			HMUI::ViewController::AnimationType::In,
			HMUI::ViewController::AnimationDirection::Vertical);
	}

	void ButtonCallback(int btnId) {
		switch (btnId)
		{
		default:
		case 0: // Back to browser
			MpModeSelection::MakeServerBrowserTopView();
			break;
		case 1: // Retry connection
			MpModeSelection::ConnectToHostedGame(MpModeSelection::LastConnectToHostedGame);
			break;
		}
	}

	void MpModeSelection::PresentConnectionFailedError(std::string errorTitle, std::string errorMessage, bool canRetry) {
		CancelLobbyJoin();

		//if (LastConnectToHostedGame == nullptr)
		if (true)
			canRetry = false; // we don't have game info to retry with
		Il2CppString* errorTitleStr = il2cpp_utils::newcsstr(errorTitle);
		Il2CppString* errorMessageStr = il2cpp_utils::newcsstr(errorMessage);
		Il2CppString* backToBrowserStr = il2cpp_utils::newcsstr("Back to browser");
		Il2CppString* canRetryStr = il2cpp_utils::newcsstr("Retry connection");
		//auto action = il2cpp_utils::MakeDelegate<System::Action_1<int>*>(classof(System::Action_1<int>*), this, std::function<void(int)>(
		//	[&] (int btnId)
		//	{
		//		switch (btnId)
		//		{
		//		default:
		//		case 0: // Back to browser
		//			MakeServerBrowserTopView();
		//			break;
		//		case 1: // Retry connection
		//			//ConnectToHostedGame(LastConnectToHostedGame);
		//			break;
		//		}
		//	}));
		auto* action = il2cpp_utils::MakeDelegate<System::Action_1<int>*>(classof(System::Action_1<int>*), std::function<void(int)>(ButtonCallback));

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