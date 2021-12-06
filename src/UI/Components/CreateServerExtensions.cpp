#include "UI/Components/CreateServerExtensions.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/RectTransform.hpp"
//#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "Game/MpSession.hpp"
#include "PluginConfig.hpp"
using namespace UnityEngine;
using UnityEngine::UI::VerticalLayoutGroup;
using namespace QuestUI;

DEFINE_TYPE(ServerBrowser::UI::Components, CreateServerExtensions);

namespace ServerBrowser::UI::Components
{
    void CreateServerExtensions::Awake() {
        wrapper = get_transform()->Find(il2cpp_utils::newcsstr("Wrapper"));
        formView = wrapper->get_transform()->Find(il2cpp_utils::newcsstr("CreateServerFormView"));

        //layout = BeatSaberUI::CreateVerticalLayoutGroup(formView);

        addToBrowserSetting = CreateToggle("Add to Server Browser", get_AddToBrowserValue(), [this](bool newValue) { OnAddToBrowserChange(newValue); });
        serverNameSetting = CreateTextInput("Server Name - " + Game::MpSession::GetDefaultHostGameName(), get_ServerNameValue(), [this](std::string_view newValue) { OnServerNameChange(newValue.data()); });

        CreateExtraText();
    }

    void CreateServerExtensions::OnEnable()
    {
        // We're working around an issue with the BSML toggle here, it fires an incorrect change event around enable-time
        // (Also, the ~150ms delay adds a nice animation when programatically changing the user's preference so this is kinda cool)

        if (firstEnable)
        {
            // Do this on first-enable to prevent the form view glitching (elements shifting and such)
            SyncValues();
            firstEnable = false;
        }

        eventsEnabled = false;

        //await Task.Delay(150);

        if (this->get_enabled())
        {
            eventsEnabled = true;
            SyncValues();
        }
    }

    void CreateServerExtensions::SyncValues()
    {
        OnAddToBrowserChange(get_AddToBrowserValue());
        OnServerNameChange(get_ServerNameValue());
        serverNameSetting->SetText(il2cpp_utils::newcsstr(Game::MpSession::GetHostGameName())); // this will update both keyboard text & button face text
    }

    void CreateServerExtensions::OnDisable()
    {
        eventsEnabled = false;
    }

    #pragma region UI Helpers
    UnityEngine::UI::Toggle* CreateServerExtensions::CreateToggle(std::string label, bool value, std::function<void(bool)> onChangeCallback, std::string hoverHint)
    {
        // Base
        //auto toggleSetting = BeatSaberUI::CreateToggle(layout->get_transform(), label, value, onChangeCallback);
        //reinterpret_cast<RectTransform*>(toggleSetting->get_transform())->set_sizeDelta({ 90.0f, 7.0f });
        //if (!hoverHint.empty())
        //    BeatSaberUI::AddHoverHint(toggleSetting->get_gameObject(), hoverHint);

        auto toggleSetting = BeatSaberUI::CreateToggle(formView, label, value, onChangeCallback);
        reinterpret_cast<RectTransform*>(toggleSetting->get_transform()->GetParent())->set_sizeDelta({ 90.0f, 7.0f });
        if (!hoverHint.empty())
            BeatSaberUI::AddHoverHint(toggleSetting->get_gameObject(), hoverHint);

        return toggleSetting;
        //var toggleTagObj = (CreateToggle()).CreateObject(_formView);
        //(toggleTagObj.transform as RectTransform).sizeDelta = Vector2(90.0f, 7.0f);
        //var toggleSetting = toggleTagObj.GetComponent<ToggleSetting>();

        // Label
        //toggleSetting.text.SetText(label);

        // Value
        //toggleSetting.toggle.isOn = value;

        // Event
        //toggleSetting.toggle.onValueChanged.RemoveAllListeners();
        //toggleSetting.toggle.onValueChanged.AddListener(delegate(bool newValue)
        //{
        //    if (_eventsEnabled)
        //    {
        //        toggleSetting.toggle.isOn = newValue;
        //        onChangeCallback(newValue);
        //    }
        //});

        //return toggleSetting;
    }

    HMUI::InputFieldView* CreateServerExtensions::CreateTextInput(std::string label, std::string value, std::function<void(std::string_view)> onChangeCallback)
    {
        // Base
        //auto stringSetting = BeatSaberUI::CreateStringSetting(layout->get_transform(), label, value, onChangeCallback);
        //reinterpret_cast<RectTransform*>(stringSetting->get_transform())->set_sizeDelta({ 90.0f, 7.0f });
        //auto layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(formView);
        //auto text = QuestUI::BeatSaberUI::CreateText(layout->get_transform(), label);
        //reinterpret_cast<RectTransform*>(text->get_transform())->set_sizeDelta({ 45.0f, 7.0f });

        //auto stringSetting = BeatSaberUI::CreateStringSetting(formView, label, value, onChangeCallback);
        //reinterpret_cast<RectTransform*>(stringSetting->get_transform()->GetParent())->set_sizeDelta({ 90.0f, 7.0f });

        //reinterpret_cast<RectTransform*>(stringSetting->get_transform()->GetParent())->set_sizeDelta({ 20.0f, 7.0f });
        //reinterpret_cast<RectTransform*>(layout->get_transform())->set_sizeDelta({ 90.0f, 7.0f });

        // This works
        auto layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(formView);
        auto stringSetting = BeatSaberUI::CreateStringSetting(layout->get_transform(), label, value, onChangeCallback);
        reinterpret_cast<RectTransform*>(layout->get_transform())->set_sizeDelta({ 90.0f, 7.0f });


        //var stringTagObj = (new StringSettingTag()).CreateObject(_formView);
        //(stringTagObj.transform as RectTransform).sizeDelta = new Vector2(90.0f, 7.0f);
        //var stringSetting = stringTagObj.GetComponent<StringSetting>();

        // Label
        //stringSetting.GetComponentInChildren<TextMeshProUGUI>().text = label;

        // Value
        //stringSetting.modalKeyboard.clearOnOpen = false;
        //stringSetting.modalKeyboard.keyboard.KeyboardText.text = value;
        //stringSetting.text.text = value;
        //stringSetting.text.richText = false;
        //stringSetting.text.alignment = TextAlignmentOptions.Center;

        // Event
        //stringSetting.modalKeyboard.keyboard.EnterPressed += (async delegate(string newValue)
        //{
        //    await Task.Delay(1); // we need to run after BSML's own EnterPressed, and this, well, it works
        //    onChangeCallback(newValue);
        //});

        // Make it look nice :-)
        //var valuePicker = stringSetting.transform.Find("ValuePicker");

        //var buttonLeftSide = valuePicker.Find("DecButton") as RectTransform;
        //var buttonRightSide = valuePicker.Find("IncButton") as RectTransform;
        //var valueText = valuePicker.Find("ValueText") as RectTransform;

        //float leftSideWidth = 0.05f;

        //buttonLeftSide.anchorMin = new Vector2(0.0f, 0.0f);
        //buttonLeftSide.anchorMax = new Vector2(leftSideWidth, 1.0f);
        //buttonLeftSide.offsetMin = new Vector2(0.0f, 0.0f);
        //buttonLeftSide.offsetMax = new Vector2(0.0f, 0.0f);
        //buttonLeftSide.sizeDelta = new Vector2(0.0f, 0.0f);

        //buttonRightSide.anchorMin = new Vector2(leftSideWidth, 0.0f);
        //buttonRightSide.anchorMax = new Vector2(1.0f, 1.0f);
        //buttonRightSide.offsetMin = new Vector2(0.0f, 0.0f);
        //buttonRightSide.offsetMax = new Vector2(0.0f, 0.0f);
        //buttonRightSide.sizeDelta = new Vector2(0.0f, 0.0f);

        //valueText.anchorMin = new Vector2(0.0f, 0.0f);
        //valueText.anchorMax = new Vector2(1.0f, 1.0f);
        //valueText.offsetMin = new Vector2(0.0f, -0.33f);
        //valueText.offsetMax = new Vector2(0.0f, 0.0f);
        //valueText.sizeDelta = new Vector2(0.0f, 0.0f);

        //var editIcon = buttonRightSide.Find("EditIcon").GetComponent<ImageView>();
        //editIcon.sprite = Sprites.Pencil;
        //editIcon.transform.localScale = new Vector3(-1.0f, -1.0f, 1.0f);

        return stringSetting;
    }

    TMPro::TextMeshProUGUI* CreateServerExtensions::CreateExtraText(std::string initialText)
    {
        auto textTagObject = BeatSaberUI::CreateText(formView, initialText);

        textTagObject->get_rectTransform()->set_offsetMin({ 0.0f, -30.0f });
        textTagObject->get_rectTransform()->set_offsetMax({ 90.0f, -30.0f });
        textTagObject->get_rectTransform()->set_sizeDelta({ 90.0f, 15.0f });
        textTagObject->set_alignment(TMPro::TextAlignmentOptions::Center);
        textTagObject->set_fontSize(4.0f);
        textTagObject->set_extraPadding(true);

        return textTagObject;
    }

    void CreateServerExtensions::ReApplyVerticalLayout(bool extraHeight)
    {
        wrapper->GetComponent<VerticalLayoutGroup*>()->set_enabled(false);
        formView->GetComponent<VerticalLayoutGroup*>()->set_enabled(false);

        reinterpret_cast<RectTransform*>(formView)->set_offsetMax({ 90.0f, 0.0f });
        reinterpret_cast<RectTransform*>(formView)->set_sizeDelta({ 90.0f, extraHeight ? 20.0f : 15.0f });

        formView->GetComponent<VerticalLayoutGroup*>()->set_enabled(true);
        wrapper->GetComponent<VerticalLayoutGroup*>()->set_enabled(true);
    }
    #pragma endregion

    #pragma region UI Events
    void CreateServerExtensions::OnAddToBrowserChange(bool newValue)
    {
        getPluginConfig().LobbyAnnounceToggle.SetValue(newValue);
        //Plugin.Config.LobbyAnnounceToggle = newValue;

        // Ensure the control is in sync
        //addToBrowserSetting->set_isOn(newValue);

        // Show server browser specific settings only if toggled on
        serverNameSetting->get_gameObject()->SetActive(newValue);
        ReApplyVerticalLayout(newValue);
    }

    void CreateServerExtensions::OnServerNameChange(std::string newValue)
    {
        if (newValue.empty())
            getPluginConfig().CustomGameName.SetValue(Game::MpSession::GetDefaultHostGameName());
        else getPluginConfig().CustomGameName.SetValue(newValue);
        //newValue = Game::MpSession::GetHostGameName(); // this will read CustomGameName but fall back to a default name if left empty
        //getPluginConfig().CustomGameName.SetValue(newValue);
        //serverNameSetting->SetText(il2cpp_utils::newcsstr(newValue)); // this will update both keyboard text & button face text
    }
    #pragma endregion

    #pragma region UI Data
    bool CreateServerExtensions::get_AddToBrowserValue()
    {
        return getPluginConfig().LobbyAnnounceToggle.GetValue();
    }

    std::string CreateServerExtensions::get_ServerNameValue()
    {
        return Game::MpSession::GetHostGameName();
    }
    #pragma endregion
}