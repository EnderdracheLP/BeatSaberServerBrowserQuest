#include "main.hpp"
#include "PluginConfig.hpp"
#include "Core/HostedGameData.hpp"
#include "Core/GlobalModState.hpp"
#include "Game/MpModeSelection.hpp"
#include "Game/MpConnect.hpp"
#include "Game/MpSession.hpp"
#include "Game/MpLocalPlayer.hpp"
#include "UI/Components/CreateServerExtensions.hpp"
#include "Utils/ConnectionErrorText.hpp"
#include "Utils/WebUtils.hpp"
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
#include "UnityEngine/Application.hpp"

#include "custom-types/shared/register.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "GlobalNamespace/BeatmapDifficultyMask.hpp"
#include "GlobalNamespace/GameplayModifierMask.hpp"
#include "GlobalNamespace/SongPackMask.hpp"
#include "GlobalNamespace/DiscoveryPolicy.hpp"
#include "GlobalNamespace/InvitePolicy.hpp"
#include "GlobalNamespace/GameplayServerMode.hpp"
#include "GlobalNamespace/SongSelectionMode.hpp"
#include "GlobalNamespace/GameplayServerControlSettings.hpp"

#include "GlobalNamespace/MasterServerConnectionManager.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionViewController.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/CreateServerViewController.hpp"
#include "GlobalNamespace/GameServerBrowserFlowCoordinator.hpp"
#include "GlobalNamespace/GameServerBrowserViewController.hpp"
#include "GlobalNamespace/MultiplayerLobbyConnectionController.hpp"
#include "GlobalNamespace/ConnectionFailedReason.hpp"
#include "GlobalNamespace/PacketEncryptionLayer.hpp"
#include "GlobalNamespace/NetworkConfigSO.hpp"

#include "GlobalNamespace/UserCertificateValidator.hpp"
#include "GlobalNamespace/PlatformAuthenticationTokenProvider.hpp"
#include "GlobalNamespace/AuthenticationToken.hpp"

#include "System/Net/IPEndPoint.hpp"
#include "System/Net/IPAddress.hpp"
#include "System/Threading/Tasks/Task_1.hpp"

#include "GlobalNamespace/BGNetDebug.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace ServerBrowser::UI;
using namespace ServerBrowser::Game;
using namespace ServerBrowser::Core;
using namespace ServerBrowser::Utils;

#define QUICK_HOOK_GB(class_, methodname, retval, ...) QUICK_HOOK(GlobalNamespace, class_, methodname, retval, __VA_ARGS__)
#define QUICK_HOOK(namespace_, class_, methodname, retval, ...) MAKE_HOOK_MATCH(class_##_##methodname, &namespace_::class_::methodname, retval, namespace_::class_* self, __VA_ARGS__)

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

DEFINE_CONFIG(PluginConfig);

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

// Function to split string `str` using a given delimiter
std::vector<std::string> split(const std::string& str, char delim)
{
    auto i = 0;
    std::vector<std::string> list;

    auto pos = str.find(delim);

    while (pos != std::string::npos)
    {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}

/// <summary>
/// This hook lets us determine the host's server secret.
/// This can then be used to help connect to specific games without a code, e.g. in case of Quickplay games.
/// </summary>
QUICK_HOOK_GB(MasterServerConnectionManager, HandleConnectToServerSuccess, void, ::Il2CppString* userId, ::Il2CppString* userName, 
    System::Net::IPEndPoint* remoteEndPoint, 
    ::Il2CppString* secret, ::Il2CppString* code, 
    GlobalNamespace::BeatmapLevelSelectionMask selectionMask, 
    GlobalNamespace::GameplayServerConfiguration configuration, 
    ::Array<uint8_t>* preMasterSecret, ::Array<uint8_t>* myRandom, ::Array<uint8_t>* remoteRandom, 
    bool isConnectionOwner, bool isDedicatedServer, ::Il2CppString* managerId) {

    getLogger().info("HandleConnectToServerSuccess(userId=%s, userName=%s, "
        " remoteEndPoint=%s, secret=%s, code=%s,"
        " isDedicatedServer=%s, managerId=%s)",
        to_utf8(csstrtostr(userId)).c_str(),
        to_utf8(csstrtostr(userName)).c_str(),
        to_utf8(csstrtostr(remoteEndPoint->ToString())).c_str(),
        to_utf8(csstrtostr(secret)).c_str(),
        to_utf8(csstrtostr(code)).c_str(),
        isDedicatedServer ? "true" : "false",
        to_utf8(csstrtostr(managerId)).c_str());

    getLogger().debug("Checking GlobalModState bools: WeInitiatedConnection: %s, LastConnectToHostedGame: %s, DirectConnectTarget: %s ", 
        GlobalModState::WeInitiatedConnection ? "true" : "false", 
        GlobalModState::LastConnectToHostedGame.has_value() ? "true" : "false", 
        GlobalModState::DirectConnectTarget.has_value() ? "true" : "false");

    if (GlobalModState::WeInitiatedConnection && GlobalModState::LastConnectToHostedGame.has_value())
    {
        // Server Browser initiated this connection attempt
        if (GlobalModState::DirectConnectTarget.has_value())
        {
            // Direct connection attempt, ignore the master server target
            getLogger().warning("HandleConnectToServerSuccess: Forcing direct connection override"
                " (DirectConnectTarget=%s)", GlobalModState::DirectConnectTarget.value().c_str());

            userId = il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_OwnerId());
            userName = il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_OwnerName());
            auto newEndpoint = split(GlobalModState::DirectConnectTarget.value(), ':');
            System::Net::IPAddress* address;
            System::Net::IPAddress::TryParse(il2cpp_utils::newcsstr(newEndpoint[0]), ByRef(address));
            remoteEndPoint = System::Net::IPEndPoint::New_ctor(address, std::stoi(newEndpoint[1]));
            secret = GlobalModState::LastConnectToHostedGame->get_HostSecret().has_value() ? il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_HostSecret().value()) : nullptr;
            code = il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_ServerCode());
            selectionMask = BeatmapLevelSelectionMask(BeatmapDifficultyMask::All,
                GameplayModifierMask::All, SongPackMask::get_all());
            configuration = GameplayServerConfiguration(GlobalModState::LastConnectToHostedGame->get_PlayerLimit(),
                DiscoveryPolicy::Public, InvitePolicy::AnyoneCanInvite, GameplayServerMode::Managed,
                SongSelectionMode::OwnerPicks, GameplayServerControlSettings::All);
            managerId = GlobalModState::LastConnectToHostedGame->get_ManagerId().has_value() ?
                il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_ManagerId().value()) :
                GlobalModState::LastConnectToHostedGame->get_OwnerId().empty() ? nullptr : il2cpp_utils::newcsstr(GlobalModState::LastConnectToHostedGame->get_OwnerId());

            GlobalModState::ShouldDisableEncryption = true; // about to talk to game server, disable encryption
        }
        else
        {
            // Normal connection attempt, verify the master server target
            auto targetGame = GlobalModState::LastConnectToHostedGame;
            auto isValidJoin = true;

            if (!targetGame->get_ServerCode().empty() && to_utf8(csstrtostr(code)) != targetGame->get_ServerCode())
            {
                // Server code mismatch
                getLogger().warning("HandleConnectToServerSuccess: Server Code mismatch"
                    " (Expected=%s, Actual=%s)", targetGame->get_ServerCode().c_str(), to_utf8(csstrtostr(code)).c_str());
                //Plugin.Log.Warn("HandleConnectToServerSuccess: Server Code mismatch" +
                //    $" (Expected={targetGame.ServerCode}, Actual={code})");
                isValidJoin = false;
            }

            if (targetGame->get_HostSecret().has_value() && !Il2CppString::IsNullOrEmpty(secret)
                && to_utf8(csstrtostr(secret)) != targetGame->get_HostSecret().value())
            {
                // Host secret mismatch
                getLogger().warning("HandleConnectToServerSuccess: Host Secret mismatch"
                " (Expected=%s, Actual=%s)", targetGame->get_HostSecret()->c_str(), to_utf8(csstrtostr(secret)).c_str());

                //Plugin.Log.Warn("HandleConnectToServerSuccess: Host Secret mismatch" +
                //    $" (Expected={targetGame.HostSecret}, Actual={secret})");
                isValidJoin = false;
            }

            if (!isValidJoin)
            {
                GlobalModState::WeAbortedJoin = true;
                MpModeSelection::CancelLobbyJoin(false);
                return;
            }
        }
    }

    // Track the server connection and update game state as needed 
    //MpEvents::RaiseBeforeConnectToServer(self, new ConnectToServerEventArgs()
        //{
        //    UserId = userId,
        //    UserName = userName,
        //    RemoteEndPoint = remoteEndPoint,
        //    Secret = secret,
        //    Code = code,
        //    SelectionMask = selectionMask,
        //    Configuration = configuration,
        //    IsDedicatedServer = isDedicatedServer,
        //    IsConnectionOwner = isConnectionOwner,
        //    ManagerId = managerId
        //});
    MasterServerConnectionManager_HandleConnectToServerSuccess(self, userId, userName, remoteEndPoint, secret, code, selectionMask, configuration, preMasterSecret, myRandom, remoteRandom, isConnectionOwner, isDedicatedServer, managerId);
}

QUICK_HOOK_GB(MultiplayerModeSelectionViewController, DidActivate, void, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
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
        // Bind multiplayer session events
        MpSession::SetUp();
        MpModeSelection::SetUp();

        // UI setup
        PluginUi::SetUp();

        // Read local player info
        MpLocalPlayer::SetUp();
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

QUICK_HOOK_GB(MultiplayerModeSelectionFlowCoordinator, HandleMultiplayerLobbyControllerDidFinish, void, MultiplayerModeSelectionViewController* viewController, MultiplayerModeSelectionViewController::MenuButton menuButton) {
    // TODO: Make sure any overrides are cleared when we're going to connect or host
    MpConnect::ClearMasterServerOverride();
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

QUICK_HOOK_GB(CreateServerViewController, DidActivate, void, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().debug("CreateServerViewController_DidActivate");
    CreateServerViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (firstActivation) {
        self->get_gameObject()->AddComponent<ServerBrowser::UI::Components::CreateServerExtensions*>();
    }
}

QUICK_HOOK_GB(MultiplayerModeSelectionFlowCoordinator, PresentConnectionErrorDialog, void, GlobalNamespace::MultiplayerLobbyConnectionController::LobbyConnectionType connectionType, GlobalNamespace::ConnectionFailedReason reason) {
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

QUICK_HOOK_GB(PacketEncryptionLayer, ProcessOutBoundPacketInternal, bool, System::Net::IPEndPoint* remoteEndPoint, ByRef<::Array<uint8_t>*> data, ByRef<int> offset, ByRef<int> length, ByRef<bool> encrypted) {
    if (GlobalModState::ShouldDisableEncryption)
    {
        // Disabling; do not perform outbound encryption, disable unencrypted traffic filter
        if (self->filterUnencryptedTraffic)
            self->filterUnencryptedTraffic = false;

        encrypted.heldRef = false;

        if (offset.heldRef == 0)
        {
            System::Array::Copy(data.heldRef, offset.heldRef, data.heldRef, offset.heldRef + 1, length.heldRef);
        }
        else
        {
            offset.heldRef--;
        }

        length.heldRef++;
        data.heldRef->values[offset.heldRef] = 0;

        return true;
    }

    // Not disabling; run regular code, re-enable the unencrypted traffic filter (regular game behavior)
    if (!self->filterUnencryptedTraffic)
        self->filterUnencryptedTraffic = true;

    encrypted.heldRef = false;
    return PacketEncryptionLayer_ProcessOutBoundPacketInternal(self, remoteEndPoint, data, offset, length, encrypted);
}

QUICK_HOOK_GB(UserCertificateValidator, ValidateCertificateChainInternal, void, GlobalNamespace::MasterServerEndPoint* endPoint, System::Security::Cryptography::X509Certificates::X509Certificate2* certificate, ::Array<::Array<uint8_t>*>* certificateChain)
{
    // This mod disables certificate validation when it is overriding the master server to an unofficial one only.
    // If we are connecting to official games, we won't interfere.

    if (MpConnect::get_ShouldDisableCertificateValidation())
    {
        getLogger().info("Bypassing user certificate validation");
    }
    else {
        UserCertificateValidator_ValidateCertificateChainInternal(self, endPoint, certificate, certificateChain);
    }
}

MAKE_HOOK_MATCH(NetworkConfigSO_get_masterServerEndPoint, &NetworkConfigSO::get_masterServerEndPoint, GlobalNamespace::MasterServerEndPoint*, NetworkConfigSO* self) {
    getLogger().debug("Running get_masterServerEndPoint");
    auto result = NetworkConfigSO_get_masterServerEndPoint(self);
    MpConnect::ReportCurrentMasterServerValue(result);

    if (MpConnect::get_OverrideEndPoint())
    {
        getLogger().debug("Overriding Endpoint in get_masterServerEndPoint");
        // We are overriding the endpoint, to replace either the official or modded value
        result = MpConnect::get_OverrideEndPoint();
    }

    //MpEvents::RaiseMasterServerChanged(self, result);
    return result;
}

MAKE_HOOK_MATCH(BGNetDebug_Log, &GlobalNamespace::BGNetDebug::Log, void, Il2CppString* message) {
    getLogger().WithContext("BGNetDebug::Log").debug("%s", to_utf8(csstrtostr(message)).c_str());
    BGNetDebug_Log(message);
}

MAKE_HOOK_MATCH(BGNetDebug_LogError, &GlobalNamespace::BGNetDebug::LogError, void, Il2CppString* message) {
    getLogger().WithContext("BGNetDebug::LogError").error("%s", to_utf8(csstrtostr(message)).c_str());
    BGNetDebug_LogError(message);
}

MAKE_HOOK_MATCH(BGNetDebug_LogWarning, &GlobalNamespace::BGNetDebug::LogWarning, void, Il2CppString* message) {
    getLogger().WithContext("BGNetDebug::LogWarning").warning("%s", to_utf8(csstrtostr(message)).c_str());
    BGNetDebug_LogWarning(message);
}

MAKE_HOOK_MATCH(PlatformAuthenticationTokenProvider_GetAuthenticationToken, &PlatformAuthenticationTokenProvider::GetAuthenticationToken, System::Threading::Tasks::Task_1<GlobalNamespace::AuthenticationToken>*, PlatformAuthenticationTokenProvider* self)
{
    getLogger().debug("Returning custom authentication token!");
    return System::Threading::Tasks::Task_1<AuthenticationToken>::New_ctor(AuthenticationToken(
        AuthenticationToken::Platform::OculusQuest,
        self->userId,
        self->userName,
        Array<uint8_t>::NewLength(0)
    ));
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
    getPluginConfig().Init(modInfo);
    custom_types::Register::AutoRegister();
    QuestUI::Init();

    WebUtils::GameVersion = to_utf8(csstrtostr(UnityEngine::Application::get_version()));
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionViewController_DidActivate);
    INSTALL_HOOK(getLogger(), CreateServerViewController_DidActivate);
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionFlowCoordinator_HandleMultiplayerLobbyControllerDidFinish);
    INSTALL_HOOK(getLogger(), MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog);
    INSTALL_HOOK(getLogger(), PacketEncryptionLayer_ProcessOutBoundPacketInternal);

    INSTALL_HOOK(getLogger(), NetworkConfigSO_get_masterServerEndPoint);
    INSTALL_HOOK(getLogger(), MasterServerConnectionManager_HandleConnectToServerSuccess);
    auto ModList = Modloader::getMods();
    if (ModList.find("BeatTogether") == ModList.end()) {
        getLogger().info("BeatTogether not found, installing our own overrides");
        INSTALL_HOOK(getLogger(), UserCertificateValidator_ValidateCertificateChainInternal);
        INSTALL_HOOK(getLogger(), PlatformAuthenticationTokenProvider_GetAuthenticationToken);
    }

    INSTALL_HOOK(getLogger(), BGNetDebug_Log);
    INSTALL_HOOK(getLogger(), BGNetDebug_LogError);
    INSTALL_HOOK(getLogger(), BGNetDebug_LogWarning);
    getLogger().info("Installed all hooks!");
}