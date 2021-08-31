#include "HostedGameCellData.hpp"

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Component.hpp"

#include "HMUI/SelectableCell.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"

#include "System/Action_2.hpp"
#include "System/Action_3.hpp"

#include <optional>

namespace ServerBrowser::UI::Components {
    using Action3 = System::Action_3<HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType, ::Il2CppObject*>*;
    //using Action2 = System::Action_2<HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType>*;
}

DECLARE_CLASS_CODEGEN(ServerBrowser::UI::Components, HostedGameCellExtensions, UnityEngine::MonoBehaviour,
    bool enabled = false;

    DECLARE_INSTANCE_FIELD(HMUI::SelectableCell*, _cell);
    HostedGameCellData _cellInfo;

    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, background);

    DECLARE_INSTANCE_FIELD(Action3, OnSelectionDidChange);
    //std::function<void(HMUI::SelectableCell*, HMUI::SelectableCell::TransitionType, ::Il2CppObject*)> fun;
    //DECLARE_INSTANCE_FIELD(Action2, OnHighlightDidChange);

    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, SongName);
    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, SongAuthor);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, FavoritesIcon);
    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, SongTime);
    DECLARE_INSTANCE_FIELD(HMUI::CurvedTextMeshPro*, SongBpm);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, BpmIcon);

    public:
    void Configure(HMUI::SelectableCell* cell, HostedGameCellData* cellInfo);
    void RefreshContent(HostedGameCellData* cellInfo = nullptr);
    private:
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);

    //DECLARE_INSTANCE_METHOD(void, OnSelectionDidChange, HMUI::SelectableCell cell, HMUI::SelectableCell::TransitionType transition/*, object _*/);
    //DECLARE_INSTANCE_METHOD(void, OnHighlightDidChange, HMUI::SelectableCell cell, HMUI::SelectableCell::TransitionType transition);
)

//namespace ServerBrowser::UI::Components {
//    class HostedGameCellExtensions {
//        bool enabled = false;
//
//        HMUI::SelectableCell* _cell;
//        HostedGameCellData _cellInfo;
//
//        void Configure(HMUI::SelectableCell* cell, HostedGameCellData cellInfo);
//        void OnEnable()
//    };
//}