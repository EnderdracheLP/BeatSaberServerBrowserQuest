#include "main.hpp"
#include "UI/ServerBrowserViewController.hpp"
#include "Core/HostedGameData.hpp"
#include "Core/HostedGameBrowser.hpp"
#include "Game/MpModeSelection.hpp"

#include "GlobalNamespace/LevelListTableCell.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/RectOffset.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/List/QuestUITableView.hpp"
#include "Utils/Sprites/carats.hpp"

#include "TMPro/TextAlignmentOptions.hpp"

#include "Polyglot/Localization.hpp"

#include "UnityEngine/Resources.hpp"
using namespace UnityEngine;
using namespace HMUI;
using namespace GlobalNamespace;
using namespace QuestUI::BeatSaberUI;
using namespace ServerBrowser::Core;
using namespace ServerBrowser::Game;
using namespace QuestUI;
using UnityEngine::UI::LayoutElement;
using UnityEngine::UI::ContentSizeFitter;
//using ServerBrowser::UI::Components::ListLoadingControl;

#include "Core/BSSBMasterAPI.hpp"

DEFINE_TYPE(ServerBrowser::UI::ViewControllers, ServerBrowserViewController);

namespace ServerBrowser::UI::ViewControllers {
    //HostedGameFilters ServerBrowserViewController::_filters = new HostedGameFilters();
    //ServerBrowserViewController::_imageLoadCancellation;
    //HostedGameData ServerBrowserViewController::_selectedGame = null;
    //ServerBrowserViewController::_loadingControl;

    //std::vector<Components::HostedGameCellData> gameCellData;

#pragma region BSML UI Components
    //[UIParams]
    //public BeatSaberMarkupLanguage.Parser.BSMLParserParams parserParams;

    //[UIComponent("mainContentRoot")]
    UnityEngine::UI::VerticalLayoutGroup* MainContentRoot;

    //[UIComponent("searchKeyboard")]
    //public ModalKeyboard SearchKeyboard;

    //[UIComponent("serverMessageText")]
    TMPro::TextMeshProUGUI* ServerMessageText;

    //[UIComponent("statusText")]
    TMPro::TextMeshProUGUI* StatusText;

    //[UIComponent("lobbyList")]
    ServerBrowser::UI::Components::CustomListTableData* GameList;

    //[UIComponent("refreshButton")]
    UnityEngine::UI::Button* RefreshButton;

    //[UIComponent("searchButton")]
    UnityEngine::UI::Button* SearchButton;

    //[UIComponent("createButton")]
    UnityEngine::UI::Button* CreateButton;

    //[UIComponent("connectButton")]
    UnityEngine::UI::Button* ConnectButton;

    //[UIComponent("pageUpButton")]
    //private Button PageUpButton;
    UnityEngine::UI::Button* PageUpButton;


    //[UIComponent("pageDownButton")]
    //private Button PageDownButton;
    UnityEngine::UI::Button* PageDownButton;

    //[UIComponent("loadingModal")]
    //HMUI::ModalView* LoadingModal;

    //[UIComponent("filterModded")]
    UnityEngine::UI::Button* FilterModdedButton;
#pragma endregion

#pragma region BSML UI Actions /*
    [UIAction("searchKeyboardSubmit")]
    private void SearchKeyboardSubmit(string text)
    {
        SearchValue = text;

        // Make main content visible again
        MainContentRoot.gameObject.SetActive(true);

        // Hit refresh
        RefreshButtonClick();
    }

    //[UIAction("refreshButtonClick")]
    */
        void ServerBrowserViewController::RefreshButtonClick()
    {
            SetInitialUiState();
            HostedGameBrowser::FullRefresh(/*_filters*/);
    }
    /*
    //[UIAction("searchButtonClick")]
    void SearchButtonClick()
    {
        ClearSelection();
        parserParams.EmitEvent("openSearchKeyboard");
    }

    //[UIAction("filterfullClick")]
    void FilterFullClick()
    {
        FilterFull = !FilterFull;
    }

    //[UIAction("filterInProgressClick")]
    void FilterInProgressClick()
    {
        FilterInProgress = !FilterInProgress;
    }

    //[UIAction("filterModdedClick")]
    void FilterModdedClick()
    {
        FilterModded = !FilterModded;
    }

    //[UIAction("createButtonClick")]
    void CreateButtonClick()
    {
        MpModeSelection.OpenCreateServerMenu();
    }

    //[UIAction("connectButtonClick")]
    void ConnectButtonClick()
    {
        //if (_selectedGame ? .CanJoin ? ? false)
        //{
        //    MpConnect.Join(_selectedGame);
        //}
        //else
        //{
        //    ClearSelection();
        //}
    }

    //[UIAction("listSelect")]
    */
    void ServerBrowserViewController::ListSelect(HMUI::TableView* tableView, int row)
    {
        if (row > GameList->data.size())
        {
            ClearSelection();
            return;
        }

        auto selectedRow = GameList->data[row];

        auto cellData = reinterpret_cast<ServerBrowser::UI::Components::HostedGameCellData*>(selectedRow);
        selectedGame = cellData->get_Game();

        ConnectButton->set_interactable(selectedGame.CanJoin());
    }
    /*
    //[UIAction("pageUpButtonClick")]
    void PageUpButtonClick()
    {
        if (HostedGameBrowser.PageIndex > 0)
        {
            SetInitialUiState();
            _ = HostedGameBrowser.LoadPage((HostedGameBrowser.PageIndex - 1) * HostedGameBrowser.PageSize, _filters);
        }
    }

    //[UIAction("pageDownButtonClick")]
    void PageDownButtonClick()
    {
        if (HostedGameBrowser.PageIndex < HostedGameBrowser.TotalPageCount - 1)
        {
            SetInitialUiState();
            _ = HostedGameBrowser.LoadPage((HostedGameBrowser.PageIndex + 1) * HostedGameBrowser.PageSize, _filters);
        }
    }
*/    #pragma endregion


#pragma region Activation / Deactivation
    void ServerBrowserViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        getLogger().debug("ServerBrowserViewController::DidActivate");
        if (firstActivation)
        {
            // TODO: Add the LevelListTableCell and such stuff for the GameList
            getLogger().debug("ServerBrowserViewController::DidActivate firstActivation");
            get_gameObject()->AddComponent<Touchable*>();

            MainContentRoot = CreateVerticalLayoutGroup(get_transform());

            // <text id="serverMessageText" text="" align="Center" font-size="3.5" anchor-min-y="2"></text>
            ServerMessageText = CreateText(get_transform(), "ServerMessageText", false);
            ServerMessageText->set_alignment(TMPro::TextAlignmentOptions::Center);
            ServerMessageText->set_fontSize(5.5f); // Default 3.5f
            ServerMessageText->get_rectTransform()->set_anchorMin({ 0.5f, 2.2f });

            UnityEngine::UI::HorizontalLayoutGroup* MainContentHorizontalLayout = CreateHorizontalLayoutGroup(MainContentRoot->get_transform());
            auto sizeFitter = MainContentHorizontalLayout->get_gameObject()->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
            sizeFitter->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
            sizeFitter->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
            MainContentHorizontalLayout->set_spacing(2);
            MainContentHorizontalLayout->get_rectTransform()->set_anchorMin({ 0, 0 });
            MainContentHorizontalLayout->get_rectTransform()->set_anchorMax({ 0.5f, 0 });

#pragma region Create GameList
            getLogger().debug("ServerBrowserViewController::DidActivate Create GameList");
            //ServerBrowser::UI::Components::CreateScrollableCustomListTableData(MainContentRoot->get_transform(), [](int index) {
            //    // TODO: Do stuff here!
            //    getLogger().debug("Cell clicked with Index: %d", index);
            //    },
            //    [this] {
            //        // TODO: Do Stuff on UP Button Press!
            //        getLogger().debug("Up Button Pressed!");
            //    },
            //    [this] {
            //    // TODO: Do Stuff on DOWN Button Press!
            //    getLogger().debug("Down Button Pressed!");
            //    }
            //);

            auto vertical = CreateVerticalLayoutGroup(MainContentRoot->get_transform());
            auto layout = vertical->get_gameObject()->AddComponent<LayoutElement*>();
            vertical->get_rectTransform()->set_sizeDelta({ 35.0f, 60.0f });
            vertical->get_rectTransform()->set_anchoredPosition({ 0.0f, 0.0f });
            vertical->set_childForceExpandHeight(false);
            vertical->set_childControlHeight(false);
            vertical->set_childScaleHeight(false);
            layout->set_preferredHeight(60.0f);
            layout->set_preferredWidth(35.0f);

            GameList = ServerBrowser::UI::Components::CreateGameList(vertical->get_transform(), { 0.0f, 0.0f }, Vector2(35.0f, 60.0f - 16.0f),
                [](int index) {
                    // TODO: Do stuff here!
                    getLogger().debug("Cell clicked with Index: %d", index);
                }
            );
            auto rect = GameList->GetComponent<RectTransform*>();

            PageUpButton = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [this]() {
                int idx = reinterpret_cast<QuestUI::TableView*>(GameList->tableView)->get_scrolledRow();
                idx -= reinterpret_cast<QuestUI::TableView*>(GameList->tableView)->get_scrollDistance();
                idx = idx > 0 ? idx : 0;
                GameList->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
                // TODO: Do Stuff on UP Button Press!
                getLogger().debug("Up Button Pressed!");
                });
            PageUpButton->get_transform()->SetAsFirstSibling();

            PageDownButton = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [this]() {
                int idx = reinterpret_cast<QuestUI::TableView*>(GameList->tableView)->get_scrolledRow();
                idx += reinterpret_cast<QuestUI::TableView*>(GameList->tableView)->get_scrollDistance();
                int max = GameList->tableView->get_dataSource()->NumberOfCells();
                idx = max <= idx ? max - 1 : idx;
                GameList->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
                // TODO: Do Stuff on DOWN Button Press!
                getLogger().debug("Down Button Pressed!");
                });
            PageDownButton->get_transform()->SetAsLastSibling();

            // replace sprites
            reinterpret_cast<RectTransform*>(PageUpButton->get_transform()->GetChild(0))->set_sizeDelta({ 8.0f, 8.0f });
            reinterpret_cast<RectTransform*>(PageDownButton->get_transform()->GetChild(0))->set_sizeDelta({ 8.0f, 8.0f });
            auto carat_up_sprite = Base64ToSprite(carat_up);
            auto carat_down_sprite = Base64ToSprite(carat_down);
            auto carat_up_inactive_sprite = Base64ToSprite(carat_up_inactive);
            auto carat_down_inactive_sprite = Base64ToSprite(carat_down_inactive);
            QuestUI::BeatSaberUI::SetButtonSprites(PageUpButton, carat_up_inactive_sprite, carat_up_sprite);
            QuestUI::BeatSaberUI::SetButtonSprites(PageDownButton, carat_down_inactive_sprite, carat_down_sprite);

            getLogger().debug("ServerBrowserViewController::DidActivate Finished Create GameList");
#pragma endregion

            UnityEngine::UI::HorizontalLayoutGroup* statusLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MainContentRoot->get_transform());
            statusLayout->get_gameObject()->AddComponent<Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
            statusLayout->get_rectTransform()->set_anchorMin({ 0, 1 });
            statusLayout->set_padding(UnityEngine::RectOffset::New_ctor(5, 5, 0, 0));
            RectTransform* statusLayoutTransform = statusLayout->GetComponent<RectTransform*>();

            ContentSizeFitter* statusLayoutContentSizeFitter = statusLayout->get_gameObject()->AddComponent<ContentSizeFitter*>();
            statusLayoutContentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

            StatusText = QuestUI::BeatSaberUI::CreateText(statusLayout->get_transform(), "Loading...", false);
            StatusText->set_alignment(TMPro::TextAlignmentOptions::Center);
            StatusText->set_fontSize(4);


            //CreateEntries(container->get_transform());

            RefreshButton = QuestUI::BeatSaberUI::CreateUIButton(MainContentHorizontalLayout->get_rectTransform(), "Refresh",
                [this]() {
                    RefreshButtonClick();
                }
            );

            CreateButton = QuestUI::BeatSaberUI::CreateUIButton(MainContentHorizontalLayout->get_rectTransform(), "Create",
                [this]() {
                    getLogger().info("Opening CreatServerMenu");
                    MpModeSelection::OpenCreateServerMenu();
                }
            );

            ConnectButton = QuestUI::BeatSaberUI::CreateUIButton(MainContentHorizontalLayout->get_rectTransform(), "Connect", "PlayButton",
                [this]() {
                    getLogger().debug("Lets connect something, too lazy now though");
                }
            );

        }
            // Attach loading control
            if (!loadingControl)
            {
                GameObject* existingLoadingControl = Resources::FindObjectsOfTypeAll<LoadingControl*>()->values[0]->get_gameObject();
                GameObject* loadingControlGameObject = UnityEngine::GameObject::Instantiate(existingLoadingControl, GameList->get_gameObject()->get_transform());
                auto loadingControlTransform = loadingControlGameObject->get_transform();
                loadingControl = loadingControlGameObject->GetComponent<LoadingControl*>();
                //loadingControl->loadingText->set_text(il2cpp_utils::newcsstr("Loading..."));
                loadingControl->set_enabled(true);
                ////std::function<void()> onRefreshButtonClick(RefreshButtonClick);
                ////_loadingControl = ListLoadingControl::Create(GameList->get_gameObject->get_transform);
                //_loadingControl = ListLoadingControl::Create(this->get_gameObject()->get_transform());
                if (loadingControl) {
                    loadingControl->didPressRefreshButtonEvent = il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*), 
                        (std::function<void()>)[this] {
                            RefreshButtonClick();
                        }
                    );
                }

            }

            // Begin listening for API browse responses
            HostedGameBrowser::OnUpdate = [this] {
                LobbyBrowser_OnUpdate();
            };

            // Reset the UI
            SetInitialUiState();

            // Perform initial refresh
            HostedGameBrowser::FullRefresh(/*_filters*/);
    }
    

    void ServerBrowserViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {

        ServerBrowser::Game::MpModeSelection::SetTitle(Polyglot::Localization::Get(il2cpp_utils::newcsstr("LABEL_MULTIPLAYER_MODE_SELECTION")));
        getLogger().debug("ServerBrowserViewController::DidDeactivate");
        CancelImageLoading(false);
    }
#pragma endregion

#pragma region Core UI Code
    void ServerBrowserViewController::SetInitialUiState() {
        getLogger().debug("ServerBrowserViewController::SetInitialUiState");

        ServerBrowser::Game::MpModeSelection::SetTitle("Server Browser");

        ClearSelection();
        CancelImageLoading();

        StatusText->set_text(il2cpp_utils::newcsstr("Loading..."));
        StatusText->set_color(Color::get_gray());

        if (loadingControl) {
            loadingControl->ShowLoading(il2cpp_utils::newcsstr("Loading servers..."));
        }
            
        // make sure the table is fully cleared, if we don't do the cell gameobjects continue to add on every load
        GameList->data.clear();
        GameList->tableView->DeleteCells(0, GameList->NumberOfCells());

        // sometimes the non-primary buttons become disabled if the server browser
        //  isn't opened until after level selection, so let's ensure they're active
        //RefreshButton->get_gameObject()->SetActive(true);
        ////SearchButton->get_gameObject()->SetActive(true);
        //CreateButton->get_gameObject()->SetActive(true);

        //RefreshButton->set_interactable(false);
        ////SearchButton->set_interactable(false);
        //ConnectButton->set_interactable(false);

        //PageUpButton->set_interactable(false);
        //PageDownButton->set_interactable(false);
    }

    void ServerBrowserViewController::CancelImageLoading(bool reset) {
        try {
            if (_imageLoadCancellation != nullptr)
            {
                _imageLoadCancellation->Cancel();
                _imageLoadCancellation->Dispose();
            }
        } catch (...) { }

        _imageLoadCancellation = nullptr;

        if (reset)
        {
            _imageLoadCancellation = THROW_UNLESS(il2cpp_utils::New<System::Threading::CancellationTokenSource*>());
        }
    }

    void ServerBrowserViewController::ClearSelection()
    {
        if (GameList && GameList->tableView) {
            GameList->tableView->ClearSelection();
            ConnectButton->set_interactable(false);
            //selectedGame;
        }
    }

    void ServerBrowserViewController::LobbyBrowser_OnUpdate()
    {
        for (auto data : GameList->data) {
            delete data;
        }
        GameList->data.clear();
        QuestUI::MainThreadScheduler::Schedule(
            [this] {
                CancelImageLoading();
            });

        if (!HostedGameBrowser::get_ServerMessage().empty())
        {
            ServerMessageText->set_text(il2cpp_utils::newcsstr(HostedGameBrowser::get_ServerMessage()));
            ServerMessageText->set_enabled(true);
        }
        else
        {
            ServerMessageText->set_enabled(false);
        }

        if (!HostedGameBrowser::get_ConnectionOk())
        {
            StatusText->set_text(il2cpp_utils::newcsstr("Failed to get server list"));
            StatusText->set_color(Color::get_red());
        }
        else if (!HostedGameBrowser::get_AnyResultsOnPage())
        {
            if (HostedGameBrowser::get_TotalResultCount() == 0)
            {
                //if (IsSearching)
                //{
                //    StatusText->set_text(i2cpp_utils::newcsstr("No servers found matching your search"));
                //}
                //else
                {
                    StatusText->set_text(il2cpp_utils::newcsstr("Sorry, no servers found"));
                }

                if (loadingControl != nullptr)
                    QuestUI::MainThreadScheduler::Schedule(
                        [this] {
                            loadingControl->ShowText(il2cpp_utils::newcsstr("No servers found"), true);
                        });
            }
            else
            {
                StatusText->set_text(il2cpp_utils::newcsstr("This page is empty"));
                RefreshButtonClick(); // this is awkward so force a refresh
            }

            StatusText->set_color(Color::get_red());
        }
        else
        {
            //StatusText.text = $"Found {HostedGameBrowser.TotalResultCount} "
            //    + (HostedGameBrowser.TotalResultCount == 1 ? "server" : "servers")
            //    + $" (Page {HostedGameBrowser.PageIndex + 1} of {HostedGameBrowser.TotalPageCount})";
            StatusText->set_text(
                il2cpp_utils::newcsstr(string_format(
            "Found %d %s "
            "(Page %d of %d)",
                HostedGameBrowser::get_TotalResultCount(), 
                HostedGameBrowser::get_TotalResultCount() == 1 ? "server" : "servers",
                HostedGameBrowser::get_PageIndex(),
                HostedGameBrowser::get_TotalPageCount()
                )));

            StatusText->set_color(Color::get_green());

            //if (IsSearching)
            //{
            //    StatusText.text += " (Filtered)";
            //}

            QuestUI::MainThreadScheduler::Schedule(
                [this] {
                    //int idx = 0;
                    for (auto& lobby : HostedGameBrowser::get_LobbiesOnPage()) {
                        GameList->data.emplace_back(reinterpret_cast<Components::CustomListTableData::CustomCellInfo*>(new Components::HostedGameCellData(_imageLoadCancellation, [this](Components::HostedGameCellData* cellInfo) { CellUpdateCallback(cellInfo); }, lobby)));
                        //GameList->CellForIdx(GameList->tableView, idx++);
                    }

                    // TODO: Remove debug code like this
                    getLogger().debug("%s", GameList->data[0]->text.c_str());
                });

            //GameList->data.data() = gameCellData.data();
            //foreach(var lobby in HostedGameBrowser.LobbiesOnPage)
            //{
            //    GameList.data.Add(new HostedGameCellData(_imageLoadCancellation, CellUpdateCallback, lobby));
            //}

            if (loadingControl != nullptr)
                QuestUI::MainThreadScheduler::Schedule(
                    [this] {
                        loadingControl->Hide();
                    });
        }

        //if (!MpSession::GetLocalPlayerHasMultiplayerExtensions())
        //{
        //    StatusText->set_text(il2cpp_utils::newcsstr(to_utf8(csstrtostr(StatusText->get_text())) + "\r\nMultiQuestensions not detected, hiding modded games"));
        //    StatusText->set_color(Color::get_yellow());
        //    //FilterModdedButton->set_interactable(false);
        //}

        AfterCellsCreated();

        RefreshButton->set_interactable(true);

        //SearchButton.interactable = (IsSearching || HostedGameBrowser.AnyResultsOnPage);
        //SearchButton.SetButtonText(IsSearching ? "<color=#32CD32>Search</color>" : "Search");

        //PageUpButton->set_interactable(HostedGameBrowser::get_PageIndex() > 0);
        //PageDownButton->set_interactable(HostedGameBrowser::get_PageIndex() < HostedGameBrowser::get_TotalPageCount() - 1);
    }
    /*
    public bool IsSearching = > _filters.AnyActive;
    #pragma endregion

    #pragma region BSML UI Bindings
    [UIValue("searchValue")]
    public string SearchValue
    {
        get = > _filters.TextSearch;
        set
        {
            _filters.TextSearch = value;
            NotifyPropertyChanged();
        }
    }

    public bool FilterFull
    {
        get = > _filters.HideFullGames;
        set
        {
            _filters.HideFullGames = value;
            NotifyPropertyChanged();
            NotifyPropertyChanged(nameof(FilterFullColor));
        }
    }

    [UIValue("filterFullColor")]
    public string FilterFullColor
    {
        get
        {
            switch (FilterFull)
            {
                case true:
                    return "#32CD32";
                case false:
                    return "#FF0000";
                default:
                    return "#FF0000";
            }
        }
    }

    public bool FilterInProgress
    {
        get = > _filters.HideInProgressGames;
        set
        {
            _filters.HideInProgressGames = value;
            NotifyPropertyChanged();
            NotifyPropertyChanged(nameof(FilterInProgressColor));
        }
    }

    [UIValue("filterInProgressColor")]
    public string FilterInProgressColor
    {
        get
        {
            switch (FilterInProgress)
            {
                case true:
                    return "#32CD32";
                case false:
                    return "#FF0000";
                default:
                    return "#FF0000";
            }
        }
    }

    public bool FilterModded
    {
        get = > _filters.HideModdedGames;
        set
        {
            _filters.HideModdedGames = value;
            NotifyPropertyChanged();
            NotifyPropertyChanged(nameof(FilterModdedColor));
        }
    }

    [UIValue("filterModdedColor")]
    public string FilterModdedColor
    {
        get
        {
            switch (FilterModded)
            {
                case true:
                    return "#32CD32";
                case false:
                    return "#FF0000";
                default:
                    return "#FF0000";
            }
        }
    }
    #pragma endregion
    */
    #pragma region Custom Cell Behaviors 

    void ServerBrowserViewController::CellUpdateCallback(Components::HostedGameCellData* cellInfo)
    {
        getLogger().debug("CellUpdateCallback called");
        GameList->tableView->RefreshCellsContent();

        //foreach(var cell in GameList.tableView.visibleCells)
        //{
        //    var extensions = cell.gameObject.GetComponent<HostedGameCellExtensions>();

        //    if (extensions != null)
        //    {
        //        extensions.RefreshContent((HostedGameCellData)GameList.data[cell.idx]);
        //    }
        //}
    }

    void ServerBrowserViewController::AfterCellsCreated()
    {
        getLogger().debug("AfterCellsCreated called");

        GameList->tableView->selectionType = TableViewSelectionType::Single;

        GameList->tableView->ReloadData(); // should cause visibleCells to be updated

        // TODO: Remove code when not needed TEMP Code for Refresh
        GameList->tableView->RefreshCells(true, true);

        //foreach(var cell in GameList.tableView.visibleCells)
        //{
        //    var extensions = cell.gameObject.GetComponent<HostedGameCellExtensions>();
        //    var data = (HostedGameCellData)GameList.data[cell.idx];

        //    if (extensions == null)
        //    {
        //        cell.gameObject.AddComponent<HostedGameCellExtensions>().Configure(cell, data);
        //    }
        //    else
        //    {
        //        extensions.RefreshContent(data);
        //    }
        //}

        ClearSelection();
    }
    #pragma endregion

}