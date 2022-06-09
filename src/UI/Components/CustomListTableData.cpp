#include "UI/Components/CustomListTableData.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/UI/ScrollRect.hpp"
#include "UnityEngine/UI/RectMask2D.hpp"
#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "System/Action_2.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/EventSystemListener.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/TableView_CellsGroup.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "questui/shared/CustomTypes/Components/List/QuestUITableView.hpp"
#include "questui/shared/BeatSaberUI.hpp"
//using namespace QuestUI;
using UnityEngine::UI::ScrollRect;
using VRUIControls::VRGraphicRaycaster;
using HMUI::Touchable;
using HMUI::EventSystemListener;

DEFINE_TYPE(ServerBrowser::UI::Components, CustomListTableData);

using namespace HMUI;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace ServerBrowser::UI::Components
{
    void CustomListTableData::ctor()
    {
        INVOKE_CTOR();
        set_listStyle(CustomListTableData::ListStyle::List);
        expandCell = true;
        reuseIdentifier = il2cpp_utils::newcsstr("ServerBrowserListTableCell");
        tableView = nullptr;
    }
    
    void CustomListTableData::dtor()
    {
        Finalize();
    }

    void CustomListTableData::set_listStyle(CustomListTableData::ListStyle value)
    {
        this->listStyle = value;
        switch (value)
        {
            case ListStyle::List:
                cellSize = 8.5f;
                break;
            case ListStyle::Box:
                cellSize = tableView->get_tableType() == TableView::TableType::Horizontal ? 30.0f : 35.0f;
                break;
            case ListStyle::Simple:
                cellSize = 8.0f;
                break;
        }
    }

    CustomListTableData::ListStyle CustomListTableData::get_listStyle()
    {
        return listStyle;
    }

    GlobalNamespace::LevelListTableCell* CustomListTableData::GetTableCell()
    {
        auto tableCell = reinterpret_cast<GlobalNamespace::LevelListTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!songListTableCellInstance)
                songListTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::LevelListTableCell*>().First([](auto x){ return x->get_name() == "LevelListTableCell"; });

            tableCell = Instantiate(songListTableCellInstance);
        }

        //tableCell.SetField("_beatmapCharacteristicImages", new Image[0]);
        tableCell->notOwned = false;

        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;   
    }

    QuestUI::QuestUIBoxTableCell* CustomListTableData::GetBoxTableCell()
    {
        auto tableCell = reinterpret_cast<QuestUI::QuestUIBoxTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!levelPackTableCellInstance)
                levelPackTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::LevelPackCell*>().First([](auto x){ return x->get_name() == "AnnotatedBeatmapLevelCollectionCell"; });
            tableCell = InstantiateBoxTableCell(levelPackTableCellInstance);
        }
        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;
    }

    QuestUI::QuestUIBoxTableCell* CustomListTableData::InstantiateBoxTableCell(GlobalNamespace::LevelPackCell* levelPackTableCell)
    {
        levelPackTableCell = Instantiate(levelPackTableCell);
        ImageView* coverImage = levelPackTableCell->coverImage;
        ImageView* selectionImage = levelPackTableCell->selectionImage;

        auto transform = coverImage->get_transform();
        for (int i = 0; i < transform->GetChildCount(); i++)
        {
            Object::Destroy(transform->GetChild(i)->get_gameObject());
        }

        GameObject* cellObject = levelPackTableCell->get_gameObject();
        Object::Destroy(levelPackTableCell);
        QuestUI::QuestUIBoxTableCell* boxTableCell = cellObject->AddComponent<QuestUI::QuestUIBoxTableCell*>();
        boxTableCell->SetComponents(coverImage, selectionImage);
        return boxTableCell;
    }

    GlobalNamespace::SimpleTextTableCell* CustomListTableData::GetSimpleTextTableCell()
    {
        auto tableCell = reinterpret_cast<GlobalNamespace::SimpleTextTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            if (!simpleTextTableCellInstance)
                simpleTextTableCellInstance = Resources::FindObjectsOfTypeAll<GlobalNamespace::SimpleTextTableCell*>().First([](auto x){ return x->get_name() == "SimpleTextTableCell"; });
            tableCell = Instantiate(simpleTextTableCellInstance);
        }

        tableCell->set_reuseIdentifier(reuseIdentifier);
        return tableCell;
    }

    HMUI::TableCell* CustomListTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        switch (listStyle)
        {
            case ListStyle::List: {
                auto tableCell = GetTableCell();

                TextMeshProUGUI* nameText = tableCell->songNameText;
                TextMeshProUGUI* authorText = tableCell->songAuthorText;
                tableCell->songBpmText->get_gameObject()->SetActive(false);
                tableCell->songDurationText->get_gameObject()->SetActive(false);
                tableCell->favoritesBadgeImage->get_gameObject()->SetActive(false);
                static auto BpmIcon = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("BpmIcon");
                tableCell->get_transform()->Find(BpmIcon)->get_gameObject()->SetActive(false);
                
                if (expandCell)
                {
                    nameText->get_rectTransform()->set_anchorMax(Vector2(2, 0.5f));
                    authorText->get_rectTransform()->set_anchorMax(Vector2(2, 0.5f));
                }

                auto& cellInfo = data[idx];
                nameText->set_text(cellInfo->get_text());
                authorText->set_text(cellInfo->get_subText());
                tableCell->coverImage->set_sprite(cellInfo->get_icon());

                return tableCell;
            }
            case ListStyle::Box: {
                // cell->set_showNewRibbon(false);
                auto& cellInfo = data[idx];
                auto cell = GetBoxTableCell();
                // cell->infoText->set_text(cellInfo->get_combinedText());
                // auto packCoverImage = cell->coverImage;
                cell->SetData(cellInfo->get_icon());

                // packCoverImage->set_sprite(cellInfo->get_icon());

                return cell;
            }
            case ListStyle::Simple: {
                auto simpleCell = GetSimpleTextTableCell();
                simpleCell->text->set_richText(true);
                simpleCell->text->set_enableWordWrapping(true);
                simpleCell->set_text(data[idx]->get_text());

                return simpleCell;
            }
        }

        return nullptr;        
    }

    float CustomListTableData::CellSize()
    {
        return cellSize;
    }

    int CustomListTableData::NumberOfCells()
    {
        return data.size();
    }

    StringW CustomListTableData::CustomCellInfo::get_text() 
    { 
        return il2cpp_utils::newcsstr(text); 
    }

    StringW CustomListTableData::CustomCellInfo::get_subText() 
    { 
        return il2cpp_utils::newcsstr(subText); 
    }

    StringW CustomListTableData::CustomCellInfo::get_combinedText()
    {
        return il2cpp_utils::newcsstr(string_format("%s\n%s", text.c_str(), subText.c_str())); 
    }

    UnityEngine::Sprite* CustomListTableData::CustomCellInfo::get_icon()
    {
        if (icon) return icon;
        auto texture = Texture2D::get_blackTexture();
        return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    }

    //CustomListTableData* CreateScrollableCustomListTableData(UnityEngine::Transform* parent, std::function<void(int)> onCellWithIdxClicked, std::function<void()> onPageUpButtonClick, std::function<void()> onPageDownButtonClick)
    //{
    //    return CreateScrollableCustomListTableData(parent, { 35.0f, 60.0f }, onCellWithIdxClicked, onPageUpButtonClick, onPageDownButtonClick);
    //}

    //CustomListTableData* CreateScrollableCustomListTableData(UnityEngine::Transform* parent, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked, std::function<void()> onPageUpButtonClick, std::function<void()> onPageDownButtonClick)
    //{
    //    return CreateScrollableCustomListTableData(parent, { 0.0f, 0.0f }, sizeDelta, onCellWithIdxClicked, onPageUpButtonClick, onPageDownButtonClick);
    //}

    //CustomListTableData* CreateScrollableCustomListTableData(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked, std::function<void()> onPageUpButtonClick, std::function<void()> onPageDownButtonClick) {
    //    auto vertical = CreateVerticalLayoutGroup(parent);
    //    auto layout = vertical->get_gameObject()->AddComponent<LayoutElement*>();
    //    vertical->get_rectTransform()->set_sizeDelta(sizeDelta);
    //    vertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
    //    vertical->set_childForceExpandHeight(false);
    //    vertical->set_childControlHeight(false);
    //    vertical->set_childScaleHeight(false);
    //    layout->set_preferredHeight(sizeDelta.y);
    //    layout->set_preferredWidth(sizeDelta.x);

    //    auto list = CreateGameList(vertical->get_transform(), Vector2(sizeDelta.x, sizeDelta.y - 16.0f), onCellWithIdxClicked);
    //    auto rect = list->GetComponent<RectTransform*>();

    //    auto up = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list]() {
    //        int idx = reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrolledRow();
    //        idx -= reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrollDistance();
    //        idx = idx > 0 ? idx : 0;
    //        list->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
    //        onPageUpButtonClick();
    //        });
    //    up->get_transform()->SetAsFirstSibling();

    //    auto down = CreateUIButton(vertical->get_transform(), "", "SettingsButton", Vector2(0.0f, 0.0f), Vector2(8.0f, 8.0f), [list]() {
    //        int idx = reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrolledRow();
    //        idx += reinterpret_cast<QuestUI::TableView*>(list->tableView)->get_scrollDistance();
    //        int max = list->tableView->get_dataSource()->NumberOfCells();
    //        idx = max <= idx ? max - 1 : idx;
    //        list->tableView->ScrollToCellWithIdx(idx, HMUI::TableView::ScrollPositionType::Beginning, true);
    //        onPageDownButtonClick();
    //        });
    //    down->get_transform()->SetAsLastSibling();

    //    // replace sprites
    //    reinterpret_cast<RectTransform*>(up->get_transform()->GetChild(0))->set_sizeDelta({ 8.0f, 8.0f });
    //    reinterpret_cast<RectTransform*>(down->get_transform()->GetChild(0))->set_sizeDelta({ 8.0f, 8.0f });
    //    auto carat_up_sprite = Base64ToSprite(carat_up);
    //    auto carat_down_sprite = Base64ToSprite(carat_down);
    //    auto carat_up_inactive_sprite = Base64ToSprite(carat_up_inactive);
    //    auto carat_down_inactive_sprite = Base64ToSprite(carat_down_inactive);
    //    SetButtonSprites(up, carat_up_inactive_sprite, carat_up_sprite);
    //    SetButtonSprites(down, carat_down_inactive_sprite, carat_down_sprite);

    //    return list;
    //}

    // BSML ListTag
    CustomListTableData* CreateGameList(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(int)> onCellWithIdxClicked)
    {
        static auto QuestUIListContainer_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ServerBrowserListContainer");
        auto container = GameObject::New_ctor(QuestUIListContainer_cs)->AddComponent<RectTransform*>();
        container->set_anchoredPosition(anchoredPosition);
        container->set_sizeDelta(sizeDelta);
        auto layoutElement = container->get_gameObject()->AddComponent<LayoutElement*>();
        layoutElement->set_flexibleHeight(sizeDelta.y);
        layoutElement->set_minHeight(sizeDelta.y);
        layoutElement->set_preferredHeight(sizeDelta.y);
        layoutElement->set_flexibleHeight(sizeDelta.x);
        layoutElement->set_minHeight(sizeDelta.x);
        layoutElement->set_preferredHeight(sizeDelta.x);

        container->SetParent(parent, false);

        static auto QuestUIList_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIList");
        auto gameObject = GameObject::New_ctor(QuestUIList_cs);
        gameObject->get_transform()->SetParent(container, false);
        gameObject->SetActive(false);

        static auto DropdownTableView_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DropdownTableView");
        auto canvasTemplate = QuestUI::ArrayUtil::First(Resources::FindObjectsOfTypeAll<Canvas*>(), [&](auto x) { return x->get_name()->Equals(DropdownTableView_cs); });

        gameObject->AddComponent<ScrollRect*>();
        auto canvas = gameObject->AddComponent<Canvas*>();
        // copy the template canvas
        canvas->set_additionalShaderChannels(canvasTemplate->get_additionalShaderChannels());
        canvas->set_overrideSorting(canvasTemplate->get_overrideSorting());
        canvas->set_pixelPerfect(canvasTemplate->get_pixelPerfect());
        canvas->set_referencePixelsPerUnit(canvasTemplate->get_referencePixelsPerUnit());
        canvas->set_renderMode(canvasTemplate->get_renderMode());
        canvas->set_scaleFactor(canvasTemplate->get_scaleFactor());
        canvas->set_sortingLayerID(canvasTemplate->get_sortingLayerID());
        canvas->set_sortingOrder(canvasTemplate->get_sortingOrder());
        canvas->set_worldCamera(canvasTemplate->get_worldCamera());

        gameObject->AddComponent<VRGraphicRaycaster*>()->physicsRaycaster = QuestUI::BeatSaberUI::GetPhysicsRaycasterWithCache();
        gameObject->AddComponent<Touchable*>();
        gameObject->AddComponent<EventSystemListener*>();
        auto scrollView = gameObject->AddComponent<ScrollView*>();

        HMUI::TableView* tableView = gameObject->AddComponent<QuestUI::TableView*>();
        auto tableData = container->get_gameObject()->AddComponent<CustomListTableData*>();
        tableData->tableView = tableView;

        tableView->preallocatedCells = Array<TableView::CellsGroup*>::NewLength(0);
        tableView->isInitialized = false;
        tableView->scrollView = scrollView;

        static auto Viewport_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Viewport");
        auto viewport = GameObject::New_ctor(Viewport_cs)->AddComponent<RectTransform*>();
        viewport->SetParent(gameObject->GetComponent<RectTransform*>(), false);
        viewport->get_gameObject()->AddComponent<RectMask2D*>();
        gameObject->GetComponent<ScrollRect*>()->set_viewport(viewport);

        static auto Content_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Content");
        auto content = GameObject::New_ctor(Content_cs)->AddComponent<RectTransform*>();
        content->SetParent(viewport, false);

        scrollView->contentRectTransform = content;
        scrollView->viewport = viewport;

        viewport->set_anchorMin({ 0.0f, 0.0f });
        viewport->set_anchorMax({ 1.0f, 1.0f });
        viewport->set_sizeDelta({ 0.0f, 0.0f });
        viewport->set_anchoredPosition({ 0.0f, 0.0f });

        auto tableViewRectT = tableView->GetComponent<RectTransform*>();
        tableViewRectT->set_anchorMin({ 0.0f, 0.0f });
        tableViewRectT->set_anchorMax({ 1.0f, 1.0f });
        tableViewRectT->set_sizeDelta({ 0.0f, 0.0f });
        tableViewRectT->set_anchoredPosition({ 0.0f, 0.0f });

        // reinterpret cast because interfaces are not explicitly inherited
        tableView->SetDataSource(reinterpret_cast<HMUI::TableView::IDataSource*>(tableData), false);

        if (onCellWithIdxClicked)
        {
            using DelegateType = System::Action_2<HMUI::TableView*, int>;
            std::function<void(HMUI::TableView*, int)> fun = [onCellWithIdxClicked](HMUI::TableView*, int idx) {
                onCellWithIdxClicked(idx);
            };
            auto delegate = il2cpp_utils::MakeDelegate<DelegateType*>(classof(DelegateType*), fun);
            tableView->add_didSelectCellWithIdxEvent(delegate);
        }

        tableView->get_gameObject()->SetActive(true);
        return tableData;
    }
}