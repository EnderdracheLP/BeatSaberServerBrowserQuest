#include "main.hpp"
//#include "UI/ServerBrowserFlowCoordinator.hpp"
//#include "UI/NativeServerBrowser.hpp"
//#include "UI/ServerBrowserViewController.hpp"
#include "Core/HostedGameData.hpp"
//#include "UI/Components/ListLoadingControl.hpp"
#include "Game/MpModeSelection.hpp"
#include "UI/PluginUi.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "GlobalNamespace/MultiplayerModeSelectionViewController.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/CreateServerViewController.hpp"
#include "GlobalNamespace/GameServerBrowserFlowCoordinator.hpp"
#include "GlobalNamespace/GameServerBrowserViewController.hpp"
using namespace GlobalNamespace;
using namespace ServerBrowser::UI;
using namespace ServerBrowser::Game;


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

    UnityEngine::UI::Button* btnGameBrowser = self->gameBrowserButton;
    btnGameBrowser->set_enabled(true);
    btnGameBrowser->get_gameObject()->SetActive(true);

    Array<UnityEngine::Component*>* comp = btnGameBrowser->GetComponents<UnityEngine::Component*>();
    //getLogger().debug("Amount of Components %d", comp->get_Length());
    for (int i = 0; i < comp->get_Length(); i++) {
        comp->values[i]->get_gameObject()->SetActive(true);
        //getLogger().debug("Components values[%d]", i);
    }

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
        transform->set_localScale(UnityEngine::Vector3(1.25f, 1.25f, 1.25f));
        btnGameBrowser->GetComponentInChildren<HMUI::CurvedTextMeshPro*>()
            ->SetText(il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Server Browser"));
#pragma endregion
    }
}


MAKE_HOOK_MATCH(MultiplayerModeSelectionViewController_HandleMenuButton, &MultiplayerModeSelectionViewController::HandleMenuButton, void, MultiplayerModeSelectionViewController* self, MultiplayerModeSelectionViewController::MenuButton menuButton) {
    getLogger().debug("MultiplayerModeSelectionViewController_HandleMenuButton");
    if (menuButton == MultiplayerModeSelectionViewController::MenuButton::GameBrowser) {
        getLogger().debug("GameBrowserButton Pressed");
    }
    /*else*/ MultiplayerModeSelectionViewController_HandleMenuButton(self, menuButton);
}

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
        MpModeSelection::WeInitiatedConnection = false;
        MpModeSelection::WeAbortedJoin = false;
    }
    MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish(self, viewController, menuButton);
}

MAKE_HOOK_MATCH(CreateServerViewController_DidActivate, &CreateServerViewController::DidActivate, void, CreateServerViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().debug("CreateServerViewController_DidActivate");

    CreateServerViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

//MAKE_HOOK_MATCH(GameServerBrowserFlowCoordinator_DidActivate, &GameServerBrowserFlowCoordinator::DidActivate, void, GameServerBrowserFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
//    getLogger().debug("GameServerBrowserFlowCoordinator_DidActivate");
//    //GameServerBrowserFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
//    self->set_showBackButton(true);
//    self->SetTitle(il2cpp_utils::newcsstr("Server Browser"), HMUI::ViewController::AnimationDirection::Vertical);
//    //self->parentFlowCoordinator->ReplaceChildFlowCoordinator(SBFlowCd, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false);
//    //self->parentFlowCoordinator->DismissFlowCoordinator(self, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
//
//    //GameServerBrowserViewController* GBVC = GameServerBrowserViewController::New_ctor();
//    //
//    //self->ProvideInitialViewControllers(GBVC, nullptr, nullptr, nullptr, nullptr);
//}

//MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameServerBrowserFlowCoordinator_BackButtonWasPressed, "", "GameServerBrowserFlowCoordinator", "BackButtonWasPressed", void, GameServerBrowserFlowCoordinator* self, HMUI::ViewController* topViewController) {
//    getLogger().debug("GameServerBrowserFlowCoordinator_BackButtonWasPressed");
//    GameServerBrowserFlowCoordinator_BackButtonWasPressed(self, topViewController);
//    self->parentFlowCoordinator->DismissFlowCoordinator(self, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
//}
//
//MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameServerBrowserViewController_DidActivate, "", "GameServerBrowserViewController", "DidActivate", void, GameServerBrowserViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
//    getLogger().debug("GameServerBrowserViewController_DidActivate");
//    GameServerBrowserViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
//}

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
    //INSTALL_HOOK(getLogger(), GameServerBrowserFlowCoordinator_DidActivate);
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish);
    //INSTALL_HOOK(getLogger(), GameServerBrowserFlowCoordinator_BackButtonWasPressed);
    //INSTALL_HOOK(getLogger(), GameServerBrowserViewController_DidActivate);
    getLogger().info("Installed all hooks!");
}