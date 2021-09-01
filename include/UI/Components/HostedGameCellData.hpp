#pragma once
#include "UnityEngine/Sprite.hpp"
#include "questui/shared/CustomTypes/Components/List/CustomListTableData.hpp"
#include "Core/HostedGameData.hpp"
#include "System/Threading/CancellationTokenSource.hpp"
#include "custom-types/shared/macros.hpp"

//DECLARE_CLASS_CUSTOM(ServerBrowser::UI::Components, HostedGameCellData, CustomListTableData::CustomCellInfo)

namespace ServerBrowser::UI::Components {
	
	class HostedGameCellData : QuestUI::CustomListTableData::CustomCellInfo {
	private:
		ServerBrowser::Core::HostedGameData Game;
		void set_Game(ServerBrowser::Core::HostedGameData GameData);

		static System::Threading::CancellationTokenSource* cancellationTokenSource;
		static std::function<void(HostedGameCellData*)> onContentChange;

		//void SetCoverArt();
		//void UpdateIcon(UnityEngine::Sprite* nextIcon);
	public:
		ServerBrowser::Core::HostedGameData get_Game();

		HostedGameCellData(System::Threading::CancellationTokenSource* CancellationTokenSource, std::function<void(HostedGameCellData*)> OnContentChange, ServerBrowser::Core::HostedGameData game);

		void UpdateUi();

	};
}