#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"

#include "HMUI/InputFieldView.hpp"

DECLARE_CLASS_CODEGEN(ServerBrowser::UI::Components, CreateServerExtensions, UnityEngine::MonoBehaviour,
        DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, wrapper);
        DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, formView);

        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, addToBrowserSetting);
        DECLARE_INSTANCE_FIELD(HMUI::InputFieldView*, serverNameSetting);

        DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, layout);

        bool eventsEnabled = true;
        bool firstEnable = true;

        DECLARE_INSTANCE_METHOD(void, Awake);
        DECLARE_INSTANCE_METHOD(void, OnEnable);
        DECLARE_INSTANCE_METHOD(void, SyncValues);
        DECLARE_INSTANCE_METHOD(void, OnDisable);
        UnityEngine::UI::Toggle* CreateToggle(std::string label, bool value, std::function<void(bool)> onChangeCallback, std::string hoverHint = "");
        HMUI::InputFieldView* CreateTextInput(std::string label, std::string value, std::function<void(std::string_view)> onChangeCallback);
        DECLARE_INSTANCE_METHOD(void, ReApplyVerticalLayout, bool extraHeight);

        void OnAddToBrowserChange(bool newValue);
        void OnServerNameChange(std::string newValue);

        bool get_AddToBrowserValue();
        std::string get_ServerNameValue();

)