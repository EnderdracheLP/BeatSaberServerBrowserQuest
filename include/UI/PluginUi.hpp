#pragma once
#include "UI/ServerBrowserViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

namespace ServerBrowser::UI
{
    class PluginUi
    {
    private:
        static ServerBrowser::UI::ViewControllers::ServerBrowserViewController* _serverBrowserViewController;
    public:
        static ServerBrowser::UI::ViewControllers::ServerBrowserViewController* get_ServerBrowserViewController();
        static void SetUp();

        static void LaunchServerBrowser();
    };
}