#pragma once
#include "main.hpp"
//#include "UI/Components/ListLoadingControl.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

#include "GlobalNamespace/LoadingControl.hpp"
#include "System/Threading/CancellationTokenSource.hpp"

DECLARE_CLASS_CODEGEN(ServerBrowser::UI::ViewControllers, ServerBrowserViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
    //private:
    //    HostedGameFilters _filters = new HostedGameFilters();
    DECLARE_INSTANCE_FIELD_DEFAULT(System::Threading::CancellationTokenSource*, _imageLoadCancellation, nullptr);
        //HostedGameData _selectedGame;
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::LoadingControl*, loadingControl, nullptr);

    DECLARE_INSTANCE_METHOD(void, RefreshButtonClick);
    DECLARE_INSTANCE_METHOD(void, SetInitialUiState);
    DECLARE_INSTANCE_METHOD(void, CancelImageLoading, bool reset = true);

    #if defined(BS__1_16) && defined(REGISTER_FUNCTION)
    REGISTER_FUNCTION(
    #elif defined(BS__1_13_2)
    REGISTER_FUNCTION(ServerBrowserController,
    #endif
    #if defined(REGISTER_METHOD)
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
#endif
);