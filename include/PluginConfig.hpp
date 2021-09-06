#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(PluginConfig,
    
    /// <summary>
    /// If enabled, when hosting a custom lobby, it will be shared to the server browser.
    /// This can be toggled from the "Create Server" dialog and the in-lobby modifiers panel.
    /// </summary>
    CONFIG_VALUE(LobbyAnnounceToggle, bool, "LobbyAnnounceToggle", true);
    //public bool LobbyAnnounceToggle{ get; set; } = true;

    /// <summary>
    /// If enabled, when joining a Quick Play lobby, it will be shared to the server browser.
    /// This can be toggled from the in-lobby modifiers panel.
    /// </summary>
    CONFIG_VALUE(ShareQuickPlayGames, bool, "ShareQuickPlayGames", true);
    //public bool ShareQuickPlayGames{ get; set; } = true;

    /// <summary>
    /// If a value is set, this name will be used when announcing the game to the server browser and master server. 
    /// </summary>
    CONFIG_VALUE(CustomGameName, std::string, "CustomGameName", "");
    //public string CustomGameName{ get; set; } = null;

    /// <summary>
    /// If enabled, overhead join/leave notifications will appear in the lobby and in multiplayer games.
    /// This can be toggled from the in-lobby modifiers panel.
    /// </summary>
    CONFIG_VALUE(JoinNotificationsEnabled, bool, "JoinNotificationsEnabled", true);
    //public bool JoinNotificationsEnabled{ get; set; } = true;

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(LobbyAnnounceToggle);
        CONFIG_INIT_VALUE(ShareQuickPlayGames);
        CONFIG_INIT_VALUE(CustomGameName);
        CONFIG_INIT_VALUE(JoinNotificationsEnabled);
    )
)