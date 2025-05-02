#pragma once

#include "rapidjson-macros/shared/macros.hpp"

namespace ServerBrowser::Models {

    /// <summary>
    /// Persistent player information that is the same between games.
    /// </summary>
    /// <see cref="BssbServerPlayer">Extended model</see>
    DECLARE_JSON_STRUCT(BssbPlayer) {
        VALUE(std::string, UserId);
        VALUE(std::string, UserName);
        VALUE(std::string, PlatformType);
        VALUE(std::string, PlatformUserId);
        // VALUE(AvatarData, AvatarData); // TODO: Create AvatarData converter
    };
}