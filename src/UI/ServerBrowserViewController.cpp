#include "UI/ServerBrowserViewController.hpp"
#include "Core/HostedGameData.hpp"
#include "Game/MpModeSelection.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "Polyglot/Localization.hpp"
#include "UnityEngine/Color.hpp"
using namespace UnityEngine;
using namespace HMUI;
using namespace GlobalNamespace;
using namespace ServerBrowser::Core;
using namespace ServerBrowser::Game;
using ServerBrowser::UI::Components::ListLoadingControl;

DEFINE_TYPE(ServerBrowser::UI::ViewControllers, ServerBrowserViewController);

namespace ServerBrowser::UI::ViewControllers {
    //HostedGameFilters ServerBrowserViewController::_filters = new HostedGameFilters();
    //System::Threading::CancellationTokenSource* ServerBrowserViewController::_imageLoadCancellation = nullptr;
    //HostedGameData ServerBrowserViewController::_selectedGame = null;
    //LoadingControl* ServerBrowserViewController::_loadingControl = nullptr;
    //UnityEngine::GameObject* GameList;

#pragma region Activation / Deactivation
    void ServerBrowserViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        getLogger().debug("ServerBrowserViewController::DidActivate");
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
            QuestUI::BeatSaberUI::CreateText(container->get_rectTransform(), "[Insert nice UI here]", false);
            //GameList = QuestUI::BeatSaberUI::CreateScrollView(container->get_rectTransform());
            getLogger().debug("ServerBrowserViewController::DidActivate firstActivation");

            // Attach loading control
            if (_loadingControl == nullptr)
            {
                //_loadingControl = ListLoadingControl::Create(GameList->get_gameObject->get_transform);
                _loadingControl = ListLoadingControl::Create(container->get_transform());
                if (_loadingControl != nullptr) {
                    //_loadingControl->didPressRefreshButtonEvent += RefreshButtonClick;
                }
            }

            // Begin listening for API browse responses
            //HostedGameBrowser::OnUpdate += LobbyBrowser_OnUpdate;

            // Reset the UI
            SetInitialUiState();

            // Perform initial refresh
            //HostedGameBrowser::FullRefresh(_filters);
        }
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

        //StatusText::text = "Loading...";
        //StatusText::color = Color::get_gray();

        if (_loadingControl != nullptr)
            _loadingControl->ShowLoading(il2cpp_utils::newcsstr("Loading servers..."));
            
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
    */
    #pragma region BSML UI Components
    //[UIParams]
    //public BeatSaberMarkupLanguage.Parser.BSMLParserParams parserParams;

    //[UIComponent("mainContentRoot")]
    //public VerticalLayoutGroup MainContentRoot;

    UnityEngine::UI::VerticalLayoutGroup* MainContentRoot;

    //[UIComponent("searchKeyboard")]
    //public ModalKeyboard SearchKeyboard;

    //[UIComponent("serverMessageText")]
    //public TextMeshProUGUI ServerMessageText;

    TMPro::TextMeshProUGUI* ServerMessageText;

    //[UIComponent("statusText")]
    //public TextMeshProUGUI StatusText;

    TMPro::TextMeshProUGUI* StatusText;

    //[UIComponent("lobbyList")]
    //public CustomListTableData GameList;

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
    //public ModalView LoadingModal;

    //[UIComponent("filterModded")]
    UnityEngine::UI::Button* FilterModdedButton;
    #pragma endregion
    /*
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

    #pragma region BSML UI Actions
    [UIAction("searchKeyboardSubmit")]
    private void SearchKeyboardSubmit(string text)
    {
        SearchValue = text;

        // Make main content visible again
        MainContentRoot.gameObject.SetActive(true);

        // Hit refresh
        RefreshButtonClick();
    }

    [UIAction("refreshButtonClick")]
    private void RefreshButtonClick()
    {
        SetInitialUiState();
        _ = HostedGameBrowser.FullRefresh(_filters);
    }

    [UIAction("searchButtonClick")]
    private void SearchButtonClick()
    {
        ClearSelection();
        parserParams.EmitEvent("openSearchKeyboard");
    }

    [UIAction("filterfullClick")]
    private void FilterFullClick()
    {
        FilterFull = !FilterFull;
    }

    [UIAction("filterInProgressClick")]
    private void FilterInProgressClick()
    {
        FilterInProgress = !FilterInProgress;
    }

    [UIAction("filterModdedClick")]
    private void FilterModdedClick()
    {
        FilterModded = !FilterModded;
    }

    [UIAction("createButtonClick")]
    private void CreateButtonClick()
    {
        MpModeSelection.OpenCreateServerMenu();
    }

    [UIAction("connectButtonClick")]
    private void ConnectButtonClick()
    {
        if (_selectedGame ? .CanJoin ? ? false)
        {
            MpConnect.Join(_selectedGame);
        }
        else
        {
            ClearSelection();
        }
    }

    [UIAction("listSelect")]
    private void ListSelect(TableView tableView, int row)
    {
        var selectedRow = GameList.data[row];

        if (selectedRow == null)
        {
            ClearSelection();
            return;
        }

        var cellData = (HostedGameCellData)selectedRow;
        _selectedGame = cellData.Game;

        ConnectButton.interactable = _selectedGame.CanJoin;
    }

    [UIAction("pageUpButtonClick")]
    private void PageUpButtonClick()
    {
        if (HostedGameBrowser.PageIndex > 0)
        {
            SetInitialUiState();
            _ = HostedGameBrowser.LoadPage((HostedGameBrowser.PageIndex - 1) * HostedGameBrowser.PageSize, _filters);
        }
    }

    [UIAction("pageDownButtonClick")]
    private void PageDownButtonClick()
    {
        if (HostedGameBrowser.PageIndex < HostedGameBrowser.TotalPageCount - 1)
        {
            SetInitialUiState();
            _ = HostedGameBrowser.LoadPage((HostedGameBrowser.PageIndex + 1) * HostedGameBrowser.PageSize, _filters);
        }
    }
    #pragma endregion

    #pragma region Custom Cell Behaviors
    private void CellUpdateCallback(HostedGameCellData cellInfo)
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

    private void AfterCellsCreated()
    {
        GameList.tableView.selectionType = TableViewSelectionType.Single;

        GameList.tableView.ReloadData(); // should cause visibleCells to be updated

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