#include "main.hpp"
#include "UI/ServerBrowserViewController.hpp"
#include "Core/HostedGameData.hpp"
#include "Game/MpModeSelection.hpp"

#include "GlobalNamespace/LevelListTableCell.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/Color.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/TableView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "TMPro/TextAlignmentOptions.hpp"

#include "Polyglot/Localization.hpp"

#include "UnityEngine/Resources.hpp"
using namespace UnityEngine;
using namespace HMUI;
using namespace GlobalNamespace;
using namespace QuestUI::BeatSaberUI;
using namespace ServerBrowser::Core;
using namespace ServerBrowser::Game;
//using ServerBrowser::UI::Components::ListLoadingControl;

DEFINE_TYPE(ServerBrowser::UI::ViewControllers, ServerBrowserViewController);

namespace ServerBrowser::UI::ViewControllers {
    //HostedGameFilters ServerBrowserViewController::_filters = new HostedGameFilters();
    //ServerBrowserViewController::_imageLoadCancellation;
    //HostedGameData ServerBrowserViewController::_selectedGame = null;
    //ServerBrowserViewController::_loadingControl;
    UnityEngine::GameObject* GameList;
    UnityEngine::UI::VerticalLayoutGroup* container;

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
    //CustomListTableData GameList;

    //CustomListTableData GameList

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

    //[UIComponent("pageDownButton")]
    //private Button PageDownButton;

    //[UIComponent("loadingModal")]
    HMUI::ModalView* LoadingModal;

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
        //_ = HostedGameBrowser.FullRefresh(_filters);
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
    void ListSelect(HMUI::TableView* tableView, int row)
    {
        auto selectedRow = GameList->data[row];

        if (selectedRow == nullptr)
        {
            ClearSelection();
            return;
        }

        auto cellData = (HostedGameCellData)selectedRow;
        _selectedGame = cellData->Game;

        ConnectButton->interactable = _selectedGame->CanJoin;
    }

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
            MainContentRoot = CreateVerticalLayoutGroup(get_rectTransform());
            //GameList = QuestUI::BeatSaberUI::CreateScrollView(MainContentRoot->get_rectTransform());

            ServerMessageText = CreateText(MainContentRoot->get_rectTransform(), "ServerMessageText", false);
            //ServerMessageText->set_alignment(TMPro::TextAlignmentOptions::Center);
            //ServerMessageText->set_fontSize(3.5f);
            //ServerMessageText->get_rectTransform()->set_anchorMin(UnityEngine::Vector2(ServerMessageText->get_rectTransform()->get_anchorMin().x, 2));

            UnityEngine::UI::HorizontalLayoutGroup* MaintContentHorizontalLayout = CreateHorizontalLayoutGroup(MainContentRoot->get_rectTransform());
            //MaintContentHorizontalLayout->set_spacing(2);
            //MaintContentHorizontalLayout->get_rectTransform()->set_anchorMin(UnityEngine::Vector2(0, MaintContentHorizontalLayout->get_rectTransform()->get_anchorMin().y));
            //MaintContentHorizontalLayout->get_rectTransform()->set_anchorMax(UnityEngine::Vector2(0.5f, MaintContentHorizontalLayout->get_rectTransform()->get_anchorMax().y));

            auto container = BeatSaberUI::CreateScrollView(get_transform());
            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_anchoredPosition(UnityEngine::Vector2(0.0f, -4.0f));
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(-54.0f, -8.0f));

            //CreateEntries(container->get_transform());

            RefreshButton = QuestUI::BeatSaberUI::CreateUIButton(MaintContentHorizontalLayout->get_rectTransform(), "Refresh", 
                [this]() {
                    RefreshButtonClick();
                }
            );

            //UnityEngine::UI::HorizontalLayoutGroup* statusPanel = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(MainContentRoot->get_rectTransform());
            //StatusText = QuestUI::BeatSaberUI::CreateText(statusPanel->get_rectTransform(), "Loading...", false);
            //StatusText->set_alignment(TMPro::TextAlignmentOptions::Center);
            //StatusText->set_fontSize(4.0f);
        }
            // Attach loading control
            if (!loadingControl)
            {
                GameObject* existingLoadingControl = Resources::FindObjectsOfTypeAll<LoadingControl*>()->values[0]->get_gameObject();
                GameObject* loadinControlGameObject = UnityEngine::GameObject::Instantiate(existingLoadingControl, this->get_gameObject()->get_transform());
                auto loadingControlTransform = loadinControlGameObject->get_transform();
                loadingControl = loadinControlGameObject->GetComponent<LoadingControl*>();
                //loadingControl->loadingText->set_text(il2cpp_utils::newcsstr("Loading..."));
                loadingControl->set_enabled(true);
                ////std::function<void()> onRefreshButtonClick(RefreshButtonClick);
                ////_loadingControl = ListLoadingControl::Create(GameList->get_gameObject->get_transform);
                //_loadingControl = ListLoadingControl::Create(this->get_gameObject()->get_transform());
                //if (_loadingControl) {
                //    //_loadingControl->didPressRefreshButtonEvent = il2cpp_utils::MakeDelegate<System::Action*>(classof(System::Action*), onRefreshButtonClick);
                //}

            }

            // Begin listening for API browse responses
            //HostedGameBrowser::OnUpdate += LobbyBrowser_OnUpdate;

            // Reset the UI
            SetInitialUiState();

            // Perform initial refresh
            //HostedGameBrowser::FullRefresh(_filters);
    }
    

    void ServerBrowserViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {

        ServerBrowser::Game::MpModeSelection::SetTitle(Polyglot::Localization::Get(il2cpp_utils::newcsstr("LABEL_MULTIPLAYER_MODE_SELECTION")));
        getLogger().debug("ServerBrowserViewController::DidDeactivate");
        CancelImageLoading(false);
    }
#pragma endregion

#pragma region Core UI Code
    void ServerBrowserViewController::SetInitialUiState() {
        ServerBrowser::Game::MpModeSelection::SetTitle("Server Browser");

        //ClearSelection();
        CancelImageLoading();

        //StatusText->set_text(il2cpp_utils::newcsstr("Loading..."));
        //StatusText->set_color(Color::get_gray());

        if (loadingControl) {
            loadingControl->ShowLoading(il2cpp_utils::newcsstr("Loading servers..."));
        }
            
        //// make sure the table is fully cleared, if we don't do the cell gameobjects continue to add on every load
        //GameList.data.Clear();
        //GameList.tableView.DeleteCells(0, GameList.tableView.numberOfCells);

        //// sometimes the non-primary buttons become disabled if the server browser
        ////  isn't opened until after level selection, so let's ensure they're active
        //RefreshButton->get_gameObject->SetActive(true);
        //SearchButton->get_gameObject->SetActive(true);
        //CreateButton->get_gameObject->SetActive(true);

        //RefreshButton->interactable = false;
        //SearchButton->interactable = false;
        //ConnectButton->interactable = false;

        //PageUpButton->interactable = false;
        //PageDownButton->interactable = false;
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

    /*
    void LobbyBrowser_OnUpdate()
    {
        GameList.data.Clear();
        CancelImageLoading();

        if (!System::String::IsNullOrEmpty(HostedGameBrowser.ServerMessage))
        {
            ServerMessageText.text = HostedGameBrowser.ServerMessage;
            ServerMessageText.enabled = true;
        }
        else
        {
            ServerMessageText.enabled = false;
        }

        if (!HostedGameBrowser.ConnectionOk)
        {
            StatusText::text = "Failed to get server list";
            StatusText::color = Color.get_red();
        }
        else if (!HostedGameBrowser::AnyResultsOnPage)
        {
            if (HostedGameBrowser::TotalResultCount == 0)
            {
                if (IsSearching)
                {
                    StatusText::text = "No servers found matching your search";
                }
                else
                {
                    StatusText::text = "Sorry, no servers found";
                }

                if (_loadingControl != nullptr)
                    _loadingControl::ShowText("No servers found", true);
            }
            else
            {
                StatusText.text = "This page is empty";
                RefreshButtonClick(); // this is awkward so force a refresh
            }

            StatusText.color = Color.get_red();
        }
        else
        {
            StatusText::text = $"Found {HostedGameBrowser.TotalResultCount} "
                + (HostedGameBrowser.TotalResultCount == 1 ? "server" : "servers")
                + $" (Page {HostedGameBrowser.PageIndex + 1} of {HostedGameBrowser.TotalPageCount})";

            StatusText.color = Color.green;

            if (IsSearching)
            {
                StatusText.text += " (Filtered)";
            }

            foreach(var lobby in HostedGameBrowser.LobbiesOnPage)
            {
                GameList.data.Add(new HostedGameCellData(_imageLoadCancellation, CellUpdateCallback, lobby));
            }

            if (_loadingControl != null)
                _loadingControl.Hide();
        }

        if (!MpSession.GetLocalPlayerHasMultiplayerExtensions())
        {
            StatusText.text += "\r\nMultiplayerExtensions not detected, hiding modded games";
            StatusText.color = Color.yellow;
            FilterModdedButton.interactable = false;
        }

        AfterCellsCreated();

        RefreshButton.interactable = true;

        SearchButton.interactable = (IsSearching || HostedGameBrowser.AnyResultsOnPage);
        SearchButton.SetButtonText(IsSearching ? "<color=#32CD32>Search</color>" : "Search");

        PageUpButton.interactable = HostedGameBrowser.PageIndex > 0;
        PageDownButton.interactable = HostedGameBrowser.PageIndex < HostedGameBrowser.TotalPageCount - 1;
    }

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

    #pragma region Custom Cell Behaviors
    void CellUpdateCallback(HostedGameCellData cellInfo)
    {
        GameList.tableView.RefreshCellsContent();

        foreach(var cell in GameList.tableView.visibleCells)
        {
            var extensions = cell.gameObject.GetComponent<HostedGameCellExtensions>();

            if (extensions != null)
            {
                extensions.RefreshContent((HostedGameCellData)GameList.data[cell.idx]);
            }
        }
    }

    void AfterCellsCreated()
    {
        GameList->tableView->selectionType = TableViewSelectionType::Single;

        GameList->tableView->ReloadData(); // should cause visibleCells to be updated

        foreach(var cell in GameList.tableView.visibleCells)
        {
            var extensions = cell.gameObject.GetComponent<HostedGameCellExtensions>();
            var data = (HostedGameCellData)GameList.data[cell.idx];

            if (extensions == null)
            {
                cell.gameObject.AddComponent<HostedGameCellExtensions>().Configure(cell, data);
            }
            else
            {
                extensions.RefreshContent(data);
            }
        }

        ClearSelection();
    }
    #pragma endregion
}
*/
}