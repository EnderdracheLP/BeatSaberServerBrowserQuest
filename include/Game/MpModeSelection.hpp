#pragma once
#include "main.hpp"
#include "Core/HostedGameData.hpp"
#include "Core/GlobalModState.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLobbyConnectionController.hpp"
#include "GlobalNamespace/JoiningLobbyViewController.hpp"
#include "GlobalNamespace/SimpleDialogPromptViewController.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "System/Action.hpp"
using ServerBrowser::Core::GlobalModState;

namespace ServerBrowser::Game {
	class MpModeSelection {
	public:
		//static bool WeInitiatedConnection;
		//static bool WeAbortedJoin;
		//static ServerBrowser::Core::HostedGameData LastConnectToHostedGame;

		// Remove the below for next update
		static StringW InjectQuickPlaySecret;
		static StringW InjectServerCode;


#pragma region Init
	private:
		static GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* _flowCoordinator;
		static GlobalNamespace::MultiplayerLobbyConnectionController* _mpLobbyConnectionController;
		static GlobalNamespace::JoiningLobbyViewController* _joiningLobbyViewController;
		static GlobalNamespace::SimpleDialogPromptViewController* _simpleDialogPromptViewController;

	public:
		static void SetUp();
#pragma endregion

#pragma region Private method helpers
		static void PresentViewController(HMUI::ViewController* viewController, HMUI::ViewController::AnimationDirection animationDirection = HMUI::ViewController::AnimationDirection::Vertical, bool immediately = false, System::Action* finishedCallback = nullptr);
		static void ReplaceTopViewController(HMUI::ViewController* viewController, HMUI::ViewController::AnimationType animationType = HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection animationDirection = HMUI::ViewController::AnimationDirection::Horizontal, System::Action* finishedCallback = nullptr);
		static void DismissViewController(HMUI::ViewController* viewController, HMUI:: ViewController::AnimationDirection animationDirection = HMUI::ViewController::AnimationDirection::Horizontal, System::Action* finishedCallback = nullptr, bool immediately = false);
		// static void SetTitle(std::string title);
		static void SetTitle(StringW title);
#pragma endregion

		static void OpenCreateServerMenu();
		static void ConnectToHostedGame(std::optional<ServerBrowser::Core::HostedGameData> game);
		static void PresentConnectionFailedError(StringW errorTitle = "Connection failed", StringW errorMessage = "", bool canRetry = true);
		// ^ set to static
		static void CancelLobbyJoin(bool hideLoading = true);
		static void MakeServerBrowserTopView();
	};
}