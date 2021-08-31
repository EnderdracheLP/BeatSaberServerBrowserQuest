#include "main.hpp"
#include "GlobalNamespace/MultiplayerLobbyState.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "Core/BSSBMasterAPI.hpp"
#include "UI/Components/HostedGameCellData.hpp"
#include "Assets/Sprites.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
using GlobalNamespace::MultiplayerLobbyState;
using ServerBrowser::Core::BSSBMasterAPI;
using ServerBrowser::Core::HostedGameData;
using ServerBrowser::Assets::Sprites;

namespace ServerBrowser::UI::Components {

    System::Threading::CancellationTokenSource* HostedGameCellData::cancellationTokenSource;
    std::function<void(HostedGameCellData*)> HostedGameCellData::onContentChange;

    HostedGameData HostedGameCellData::get_Game() {
        return Game;
    }
    void HostedGameCellData::set_Game(HostedGameData GameData) {
        Game = GameData;
    }

    HostedGameCellData::HostedGameCellData(System::Threading::CancellationTokenSource* CancellationTokenSource, std::function<void(HostedGameCellData*)> OnContentChange, ServerBrowser::Core::HostedGameData game) : QuestUI::CustomListTableData::CustomCellInfo{ "A game", "Getting details...", Sprites::get_BeatSaverIcon() } {
        cancellationTokenSource = CancellationTokenSource;
        onContentChange = OnContentChange;

        Game = game;

        UpdateUi();
    }

    void HostedGameCellData::UpdateUi() {
        text = Game.get_GameName();

        if (Game.IsOnCustomMaster()) {
            text += string_format(" <color=#59b0f4><size=3>(%s)</size></color>", Game.get_MasterServerHost().c_str());
        }

        if (Game.IsDedicatedServer()) {
            text = string_format("<color=#f39c12>%s</color>", text.c_str());
        }

        subText = "";

        if (Game.get_LobbyState() == MultiplayerLobbyState::GameRunning && Game.get_LevelId().has_value()) {
            if (!Game.get_SongAuthor().value().empty()) {
                subText += Game.get_SongAuthor().value() + " - ";
            }

            subText += Game.get_SongName().value();
        }
        else {
            subText += "In lobby";
        }

        if (Game.get_Difficulty().has_value() && Game.get_LevelId().has_value() && !Game.get_LevelId().value().empty() || Game.IsQuickPlayServer()) {
            subText += string_format(" (%s)", Game.DescribeDifficulty(true).c_str());
        }

        //try {
            SetCoverArt();
        //}
        //catch (const std::exception& ex) {
        //    getLogger().error("Could not set cover art for level%s: %s", Game.get_LevelId().value().c_str(), ex.what());
        //}
        getLogger().debug("Done with UpdateUi in HostedGameCellData");
    }

    void HostedGameCellData::SetCoverArt() {
        getLogger().debug("Running SetCoverArt in HostedGameCellData");
        if (Game.get_LevelId().has_value()) {
            getLogger().debug("Game has LevelID: %s", Game.get_LevelId()->c_str());
        }
        else {
            getLogger().debug("Game has no levelID");
        }
        getLogger().debug("LobbyState is: %d", Game.get_LobbyState());
        if (!Game.get_LevelId().has_value() || Game.get_LobbyState() != MultiplayerLobbyState::GameRunning) {
            // No level info / we are in a lobby
            getLogger().info("No level info, assuming they're in a lobby");
            UpdateIcon(Sprites::get_PortalUser());
            return;
        }

        //BSSBMasterAPI::GetCoverImageAsync(Game,
        //    [this](std::vector<uint8_t> coverArtBytes) {
        //        QuestUI::MainThreadScheduler::Schedule(
        //            [this, coverArtBytes] {
        //                std::vector<uint8_t> coverVector = coverArtBytes;
        //                auto coverArtSprite = QuestUI::BeatSaberUI::ArrayToSprite(il2cpp_utils::vectorToArray(coverVector));
        //                if (coverArtSprite) {
        //                    // Official level, or installed custom level found
        //                    UpdateIcon(coverArtSprite);
        //                    return;
        //                }
        //                else {
        //                    getLogger().error("Failed to get level info");
        //                    // Failed to get level info, show beatsaver icon as placeholder
        //                    UpdateIcon(Sprites::get_BeatSaverIcon());
        //                }
        //            });
        //    }
        //);
    }
    
    void HostedGameCellData::UpdateIcon(UnityEngine::Sprite* nextIcon) {
        getLogger().debug("Running UpdateIcon"); 
        if ((icon == nullptr || icon != nextIcon) && nextIcon) {
            getLogger().debug("Updating Icon");
            icon = nextIcon;
            getLogger().debug("Set new Icon");
            //getLogger().debug("Calling onContentChange");
            //onContentChange(this);
        }
        else {
            getLogger().debug("Did not update Icon");
        }
    }
}