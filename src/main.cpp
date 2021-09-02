#include "main.hpp"
//#include "UI/ServerBrowserFlowCoordinator.hpp"
//#include "UI/NativeServerBrowser.hpp"
//#include "UI/ServerBrowserViewController.hpp"
#include "Core/HostedGameData.hpp"
//#include "UI/Components/ListLoadingControl.hpp"
#include "Core/GlobalModState.hpp"
#include "Game/MpModeSelection.hpp"
#include "Utils/ConnectionErrorText.hpp"
#include "UI/PluginUi.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"

#include "TMPro/TextMeshProUGUI.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Component.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "GlobalNamespace/MultiplayerModeSelectionViewController.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/CreateServerViewController.hpp"
#include "GlobalNamespace/GameServerBrowserFlowCoordinator.hpp"
#include "GlobalNamespace/GameServerBrowserViewController.hpp"
#include "GlobalNamespace/MultiplayerLobbyConnectionController.hpp"
#include "GlobalNamespace/ConnectionFailedReason.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace ServerBrowser::UI;
using namespace ServerBrowser::Game;
using namespace ServerBrowser::Core;
using namespace ServerBrowser::Utils;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

//ServerBrowser::UI::ServerBrowserFlowCoordinator* SBFlowCd;

MAKE_HOOK_MATCH(MultiplayerModeSelectionViewController_DidActivate, &MultiplayerModeSelectionViewController::DidActivate, void, MultiplayerModeSelectionViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().debug("MultiplayerModeSelectionViewController_DidActivate");
    MultiplayerModeSelectionViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    //getLogger().debug("MultiplayerModeSelectionViewController get gameBrowserButtom");
    UnityEngine::UI::Button* btnGameBrowser = self->gameBrowserButton;
    btnGameBrowser->set_enabled(true);
    btnGameBrowser->get_gameObject()->SetActive(true);

    Array<Component*>* comp = btnGameBrowser->GetComponents<Component*>();
    for (int i = 0; i < comp->get_Length(); i++) {
        comp->values[i]->get_gameObject()->SetActive(true);
        //getLogger().debug("Components values[%d] name: %s", i, to_utf8(csstrtostr(comp->values[i]->ToString())).c_str());
    }


    //getLogger().debug("MultiplayerModeSelectionViewController firstActivation");
    if (firstActivation) {
#pragma region Setup
        //MpSession::Setup();
        MpModeSelection::SetUp();
        PluginUi::SetUp();
#pragma endregion

#pragma region Button Setup
        UnityEngine::Transform* transform = btnGameBrowser->get_gameObject()->get_transform();
        transform->set_position(
            UnityEngine::Vector3(
            transform->get_position().x,
            transform->get_position().y + 0.4f,
            transform->get_position().z
        ));
        transform->set_localScale({ 1.25f, 1.25f, 1.25f });
        btnGameBrowser->GetComponentInChildren<HMUI::CurvedTextMeshPro*>()
            ->SetText(il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Server Browser"));
#pragma endregion
    }
}

//MAKE_HOOK_MATCH(MultiplayerModeSelectionViewController_HandleMenuButton, &MultiplayerModeSelectionViewController::HandleMenuButton, void, MultiplayerModeSelectionViewController* self, MultiplayerModeSelectionViewController::MenuButton menuButton) {
//    getLogger().debug("MultiplayerModeSelectionViewController_HandleMenuButton");
//    if (menuButton == MultiplayerModeSelectionViewController::MenuButton::GameBrowser) {
//        getLogger().debug("GameBrowserButton Pressed");
//    }
//    /*else*/ MultiplayerModeSelectionViewController_HandleMenuButton(self, menuButton);
//}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish, &MultiplayerModeSelectionFlowCoordinator::HandleMultiplayerLobbyControllerDidFinish, void, MultiplayerModeSelectionFlowCoordinator* self, MultiplayerModeSelectionViewController* viewController, MultiplayerModeSelectionViewController::MenuButton menuButton) {
    // TODO: Make sure any overrides are cleared when we're going to connect or host
    // MpConnect.ClearMasterServerOverride();
    if (menuButton == MultiplayerModeSelectionViewController::MenuButton::GameBrowser) {
        // When the "GameBrowser" button is clicked, bypass the game's own incomplete code & open our view instead
        PluginUi::LaunchServerBrowser();
        //self->PresentViewController(ServerBrowser::UI::ViewControllers::ServerBrowserViewController, HMUI::ViewController::AnimationDirection::Horizontal);
        return;
    }
    else {
        // Going to a non-serverbrowser part of the online menu
        GlobalModState::Reset();
    }
    MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish(self, viewController, menuButton);
}

MAKE_HOOK_MATCH(CreateServerViewController_DidActivate, &CreateServerViewController::DidActivate, void, CreateServerViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().debug("CreateServerViewController_DidActivate");

    CreateServerViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_MATCH(MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog, &MultiplayerModeSelectionFlowCoordinator::PresentConnectionErrorDialog, void, MultiplayerModeSelectionFlowCoordinator* self, GlobalNamespace::MultiplayerLobbyConnectionController::LobbyConnectionType connectionType, GlobalNamespace::ConnectionFailedReason reason) {
    getLogger().warning("Multiplayer connection failed, reason: %d", reason.value);

    if (GlobalModState::WeInitiatedConnection) {
        GlobalModState::ShouldDisableEncryption = false; // always re-enable encryption for master server

        if (GlobalModState::WeAbortedJoin) {
            MpModeSelection::PresentConnectionFailedError("Connection failed", "The selected server instance is no longer available.");
            return;
        }

        if (reason == ConnectionFailedReason::ConnectionCanceled) {
            // ...and if it's just a self-cancel, return to the browser immediately.
            MpModeSelection::CancelLobbyJoin();
            MpModeSelection::MakeServerBrowserTopView();
        }
        else
        {
            MpModeSelection::PresentConnectionFailedError
            (
                "Connection failed",
                ConnectionErrorText::Generate(reason),
                reason != ConnectionFailedReason::InvalidPassword
                && reason != ConnectionFailedReason::VersionMismatch
            );
        }
        return;
    }
    else {
        MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog(self, connectionType, reason);
    }
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    QuestUI::Init();

    //SBFlowCd = QuestUI::BeatSaberUI::CreateFlowCoordinator<ServerBrowser::UI::ServerBrowserFlowCoordinator*>();
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionViewController_DidActivate);
    INSTALL_HOOK(getLogger(), CreateServerViewController_DidActivate);
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish);
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog);
    getLogger().info("Installed all hooks!");
}