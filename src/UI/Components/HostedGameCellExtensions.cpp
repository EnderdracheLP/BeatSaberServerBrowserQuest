#include "UI/Components/HostedGameCellExtensions.hpp"
#include "Core/BSSBMasterAPI.hpp"
#include "Assets/Sprites.hpp"

#include "TMPro/TextMeshProUGUI.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Color.hpp"

#include "GlobalNamespace/MultiplayerLobbyState.hpp"

#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/BeatSaberUI.hpp"
using namespace HMUI;
using namespace GlobalNamespace;
using UnityEngine::Color;
using ServerBrowser::Core::BSSBMasterAPI;
using ServerBrowser::Assets::Sprites;

DEFINE_TYPE(ServerBrowser::UI::Components, HostedGameCellExtensions);

namespace ServerBrowser::UI::Components {
#pragma region Setup
    void HostedGameCellExtensions::Configure(SelectableCell* cell, HostedGameCellData* cellInfo) {
        _cell = cell;
        _cellInfo = *cellInfo;
        CoverImageSet = false;

        background = cell->get_transform()->Find(il2cpp_utils::newcsstr("Background"))->GetComponent<ImageView*>();
        coverImage = cell->get_transform()->Find(il2cpp_utils::newcsstr("CoverImage"))->GetComponent<ImageView*>();

        std::function<void(HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType, ::Il2CppObject*)> fun = [this](SelectableCell* cell, SelectableCell::TransitionType transition, Il2CppObject* obj) { RefreshContent(); };
        OnSelectionDidChange = il2cpp_utils::MakeDelegate<System::Action_3<HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType, ::Il2CppObject*>*>(classof(System::Action_3<HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType, ::Il2CppObject*>*), fun);

        //OnSelectionDidChange = il2cpp_utils::MakeDelegate<System::Action_3<SelectableCell*, SelectableCell::TransitionType, Il2CppObject*>*>(
        //    classof(System::Action_3<SelectableCell*, SelectableCell::TransitionType, Il2CppObject*>*),
        //    [this](SelectableCell* cell, SelectableCell::TransitionType transition, Il2CppObject* obj) { RefreshContent(); });

        if (enabled) {
            OnEnable();
        }
    }
#pragma endregion

#pragma region Events
    void HostedGameCellExtensions::OnEnable() {
        if (_cell != nullptr) {
            //_cell->add_highlightDidChangeEvent(OnHighlightDidChange);
            _cell->add_selectionDidChangeEvent(OnSelectionDidChange);

            SongName = _cell->get_transform()->Find(il2cpp_utils::newcsstr("SongName"))->GetComponent<CurvedTextMeshPro*>();
            SongAuthor = _cell->get_transform()->Find(il2cpp_utils::newcsstr("SongAuthor"))->GetComponent<CurvedTextMeshPro*>();
            FavoritesIcon = _cell->get_transform()->Find(il2cpp_utils::newcsstr("FavoritesIcon"))->GetComponent<ImageView*>();
            SongTime = _cell->get_transform()->Find(il2cpp_utils::newcsstr("SongTime"))->GetComponent<CurvedTextMeshPro*>();
            SongBpm = _cell->get_transform()->Find(il2cpp_utils::newcsstr("SongBpm"))->GetComponent<CurvedTextMeshPro*>();
            BpmIcon = _cell->get_transform()->Find(il2cpp_utils::newcsstr("BpmIcon"))->GetComponent<ImageView*>();

            SongAuthor->set_richText(true);

            // Re-align BPM text and allow more horizontal space - we use this for extended lobby type
            auto songBpmTransform = reinterpret_cast<UnityEngine::RectTransform*>(SongBpm->get_transform());
            songBpmTransform->set_anchorMax({ 1.03f, 0.5f });
            songBpmTransform->set_offsetMin({ -32.00f, -4.60f });

            // Limit text size for server name and song name
            reinterpret_cast<UnityEngine::RectTransform*>(SongName->get_transform())->set_anchorMax({ 0.8f, 0.5f });
            reinterpret_cast<UnityEngine::RectTransform*>(SongAuthor->get_transform())->set_anchorMax({ 0.8f, 0.5f });

            // Allow bigger player count size (just in case we get those fat 100/100 lobbies)
            reinterpret_cast<UnityEngine::RectTransform*>(SongTime->get_transform())->set_offsetMin({ -13.0f, -2.3f });

            RefreshContent();
        }
        enabled = true;
    }

    void HostedGameCellExtensions::OnDisable() {
        enabled = false;

        if (_cell != nullptr) {
            //_cell->remove_highlightDidChangeEvent(OnHighlightDidChange);
            //_cell->remove_selectionDidChangeEvent(OnSelectionDidChange);
        }
    }

    //void HostedGameCellExtensions::OnSelectionDidChange(SelectableCell* cell, SelectableCell::TransitionType transition) {
    //    RefreshContent()
    //}

    //void HostedGameCellExtensions::OnHighlightDidChange(SelectableCell* cell, SelectableCell::TransitionType transition) {
    //}

#pragma endregion
    void HostedGameCellExtensions::RefreshContent(HostedGameCellData* cellInfo) {
        if (cellInfo) {
            _cellInfo = *cellInfo;
            CoverImageSet = false;
        }

        FavoritesIcon->get_gameObject()->SetActive(false);
        SongTime->get_gameObject()->SetActive(true);
        SongBpm->get_gameObject()->SetActive(true);
        BpmIcon->get_gameObject()->SetActive(false);

        auto game = _cellInfo.get_Game();

        if (coverImage && !CoverImageSet) {
            //getLogger().debug("Running SetCoverArt in HostedGameCellExtensions");
            //getLogger().debug("LobbyState is: %d", game.get_LobbyState());
            if (!game.get_LevelId().has_value() || game.get_LobbyState() != MultiplayerLobbyState::GameRunning) {
                // No level info / we are in a lobby
                getLogger().info("No level info, assuming they're in a lobby");
                coverImage->set_sprite(Sprites::get_PortalUser());
                CoverImageSet = true;
                goto CoverFinished;
            }
            else {
                BSSBMasterAPI::GetCoverImageAsync(game,
                    [this](std::vector<uint8_t> coverArtBytes) {
                        QuestUI::MainThreadScheduler::Schedule(
                            [this, coverArtBytes] {
                                std::vector<uint8_t> coverVector = coverArtBytes;
                                auto coverArtSprite = QuestUI::BeatSaberUI::ArrayToSprite(il2cpp_utils::vectorToArray(coverVector));
                                if (coverArtSprite) {
                                    // Official level, or installed custom level found
                                    coverImage->set_sprite(coverArtSprite);
                                    CoverImageSet = true;
                                }
                                else {
                                    getLogger().error("Failed to get level info");
                                    // Failed to get level info, show beatsaver icon as placeholder
                                    coverImage->set_sprite(Sprites::get_BeatSaverIcon());
                                }
                            });
                    }
                );
            }
        }
    CoverFinished:

        // Player count
        SongTime->set_text(il2cpp_utils::newcsstr(string_format("%d/%d", game.get_PlayerCount(), game.get_PlayerLimit())));
        SongTime->set_fontSize(4);

        // Lobby type (server + modded/vanilla indicator)
        SongBpm->set_text(game.DescribeType());

        // Text colors
        if (_cell->selected)
        {
            // Selected /w blue background: More contrasting color needed
            SongTime->set_color(Color::get_white());
            SongBpm->set_color(Color::get_white());
        }
        else
        {
            SongTime->set_color(game.get_PlayerCount() >= game.get_PlayerLimit() ? Color::get_gray() : Color::get_white());
            SongBpm->set_color(game.get_IsModded() ? Color(143.0f / 255.0f, 72.0f / 255.0f, 219.0f / 255.0f, 1.0f)
                : Color(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f, 1.0f));
        }
    }
}