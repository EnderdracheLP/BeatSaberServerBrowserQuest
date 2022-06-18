#include "main.hpp"
#include "Game/MpLocalPlayer.hpp"
#include "GlobalNamespace/IPlatformUserModel.hpp"
#include "UnityEngine/Resources.hpp"
#include "System/Action_1.hpp"
#include "System/Threading/Tasks/Task.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
using GlobalNamespace::LocalNetworkPlayerModel;
using GlobalNamespace::UserInfo;
using UnityEngine::Resources;

namespace ServerBrowser::Game {
    LocalNetworkPlayerModel* MpLocalPlayer::localNetworkPlayerModel;

    UserInfo* MpLocalPlayer::UserInfo;

    UserInfo* MpLocalPlayer::get_UserInfo() {
        return UserInfo;
    }

    UserInfo::Platform MpLocalPlayer::get_Platform() {
        if (UserInfo)
            return UserInfo->platform;
        else {
            UserInfo::Platform response(4);
            return response;
        }
    }

    const std::string MpLocalPlayer::get_PlatformId() {
        switch (get_Platform()) {
        case UserInfo::Platform::Oculus:
            return "Oculus";
        case UserInfo::Platform::Steam:
            return "Steam";
        case UserInfo::Platform::PS4:
            return "PS4";
        case UserInfo::Platform::Test:
            return "Test";
        default:
            return "unknown";
        }
    }

    void MpLocalPlayer::SetUp() {
        getLogger().debug("Setting up MpLocalPlayer");
        // Note: The game creates one local player in MainSystemInit.InstallBindings()

        localNetworkPlayerModel = Resources::FindObjectsOfTypeAll<LocalNetworkPlayerModel*>()->get(0);

        //auto platformUserModel = localNetworkPlayerModel.GetField<IPlatformUserModel, LocalNetworkPlayerModel>("_platformUserModel");
        auto UserInfoTask = localNetworkPlayerModel->platformUserModel->GetUserInfo();
        static System::Action_1<System::Threading::Tasks::Task*>* action;
        static bool gotPlayerInfo = false;
        if (!gotPlayerInfo) action = il2cpp_utils::MakeDelegate<System::Action_1<System::Threading::Tasks::Task*>*>(classof(System::Action_1<System::Threading::Tasks::Task*>*), (std::function<void(System::Threading::Tasks::Task_1<GlobalNamespace::UserInfo*>*)>)[&](System::Threading::Tasks::Task_1<GlobalNamespace::UserInfo*>* userInfoTask) {
                UserInfo = userInfoTask->get_Result();
                if (UserInfo == nullptr)
                {
                    getLogger().error("Failed to get local network player!");
                    return;
                }
        }
        );
        if (action) { 
            reinterpret_cast<System::Threading::Tasks::Task*>(UserInfoTask)->ContinueWith(action);
        }
    }

    //void MpLocalPlayer::SetUserInfo(GlobalNamespace::UserInfo* userInfo) {
    //    if (userInfo == nullptr)
    //    {
    //        getLogger().error("Failed to get local network player!");
    //        return;
    //    }
    //    MpLocalPlayer::UserInfo = userInfo;
    //    getLogger().info("Got local network player (platform: %d, platformUserId: %s, userName: %s)",
    //        UserInfo->platform.value, to_utf8(csstrtostr(UserInfo->platformUserId)).c_str(), to_utf8(csstrtostr(UserInfo->userName)).c_str());
    //}
}