#include "GlobalNamespace/LocalNetworkPlayerModel.hpp"
#include "GlobalNamespace/UserInfo.hpp"

namespace ServerBrowser::Game {
    class MpLocalPlayer {
        static GlobalNamespace::LocalNetworkPlayerModel* localNetworkPlayerModel;

        static GlobalNamespace::UserInfo* UserInfo;

        //static void SetUserInfo(GlobalNamespace::UserInfo* userInfo);
    public:
        static GlobalNamespace::UserInfo* get_UserInfo();

        static GlobalNamespace::UserInfo::Platform get_Platform();

        static std::string get_PlatformId();

        static void SetUp();
    };
}