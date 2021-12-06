#pragma once
#include "main.hpp"
//#include "UI/Components/ListLoadingControl.hpp"
#include "UI/Components/CustomListTableData.hpp"
#include "UI/Components/HostedGameCellData.hpp"
#include "custom-types/shared/macros.hpp"
#include "Core/HostedGameData.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/TableView.hpp"

#include "GlobalNamespace/LoadingControl.hpp"
#include "System/Threading/CancellationTokenSource.hpp"

DECLARE_CLASS_CODEGEN(ServerBrowser::UI::ViewControllers, ServerBrowserViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
    //private:
    //    HostedGameFilters _filters = new HostedGameFilters();
    DECLARE_INSTANCE_FIELD_DEFAULT(System::Threading::CancellationTokenSource*, _imageLoadCancellation, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::LoadingControl*, loadingControl, nullptr);

    DECLARE_INSTANCE_METHOD(void, LobbyBrowser_OnUpdate);
    DECLARE_INSTANCE_METHOD(void, RefreshButtonClick);
    DECLARE_INSTANCE_METHOD(void, AfterCellsCreated);
    DECLARE_INSTANCE_METHOD(void, ConnectButtonClick);
    DECLARE_INSTANCE_METHOD(void, ListSelect, HMUI::TableView* tableView, int row);
    DECLARE_INSTANCE_METHOD(void, PageUpButtonClick);
    DECLARE_INSTANCE_METHOD(void, PageDownButtonClick);
    DECLARE_INSTANCE_METHOD(void, SetInitialUiState);
    DECLARE_INSTANCE_METHOD(void, CancelImageLoading, bool reset = true);
    DECLARE_INSTANCE_METHOD(void, ClearSelection);

    void CellUpdateCallback(Components::HostedGameCellData* cellInfo);
    std::optional<ServerBrowser::Core::HostedGameData> selectedGame;

);