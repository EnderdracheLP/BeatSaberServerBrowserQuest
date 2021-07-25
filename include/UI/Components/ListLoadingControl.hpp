#include "main.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/LoadingControl.hpp"

namespace ServerBrowser::UI::Components {
	class ListLoadingControl {
	private:
		static UnityEngine::Transform* _controlTemplate;
		static void LoadTemplate();
	public:
		static GlobalNamespace::LoadingControl* Create(UnityEngine::Transform* parentTransform);
	};
}