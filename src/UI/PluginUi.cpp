#include "Game/MpModeSelection.hpp"
#include "UI/PluginUi.hpp"
#include "UI/ServerBrowserViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
using namespace ServerBrowser::Game;
using namespace ServerBrowser::UI::ViewControllers;
using namespace QuestUI;
using namespace HMUI;

namespace ServerBrowser::UI
{
    ServerBrowserViewController* PluginUi::_serverBrowserViewController;

        ServerBrowserViewController* PluginUi::get_ServerBrowserViewController()
        {
            if (_serverBrowserViewController == nullptr)
            {
                _serverBrowserViewController = BeatSaberUI::CreateViewController<ServerBrowserViewController*>();
            }

            return _serverBrowserViewController;
        }

        void PluginUi::SetUp()
        {
            // TODO: Add FloatingNotification
            //FloatingNotification.SetUp();
        }

        void PluginUi::LaunchServerBrowser()
        {
            MpModeSelection::PresentViewController(get_ServerBrowserViewController(), ViewController::AnimationDirection::Horizontal);
        }
}