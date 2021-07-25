#include "UI/Components/ListLoadingControl.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/GameServersListTableView.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;

namespace ServerBrowser::UI::Components {

	Transform* ListLoadingControl::_controlTemplate = nullptr;

	void ListLoadingControl::LoadTemplate() {
		if (_controlTemplate) {
			return;
		}
		GameServersListTableView* nativeGameServerList = Object::FindObjectOfType<GameServersListTableView*>();

		if (!nativeGameServerList) {
			return;
		}

		Il2CppString* MainLoadingControl = il2cpp_utils::newcsstr("TableView/Viewport/MainLoadingControl");
		_controlTemplate = nativeGameServerList->get_gameObject()->get_transform()->Find(MainLoadingControl);
	}
	LoadingControl* ListLoadingControl::Create(Transform* parentTransform) {
		LoadTemplate();

		if (!_controlTemplate) {
			return nullptr;
		}

		Transform* newObject = Object::Instantiate(_controlTemplate, parentTransform, false);

		LoadingControl* loadingControl = newObject->GetComponent<LoadingControl*>();

		loadingControl->get_gameObject()->SetActive(true);
		loadingControl->Hide();

		return loadingControl;
	}
}