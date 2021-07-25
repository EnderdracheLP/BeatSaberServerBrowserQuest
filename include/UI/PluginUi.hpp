#include "UI/ServerBrowserViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
using namespace ServerBrowser::UI::ViewControllers;
using namespace QuestUI;
using namespace HMUI;

namespace ServerBrowser::UI
{
    class PluginUi
    {
    private:
        static ServerBrowserViewController* _serverBrowserViewController;
    public:
        static ServerBrowserViewController* get_ServerBrowserViewController();
        static void SetUp();

        static void LaunchServerBrowser();
    };
}