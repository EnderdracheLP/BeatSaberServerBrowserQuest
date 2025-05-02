#pragma once

#include "rapidjson-macros/shared/macros.hpp"

namespace ServerBrowser::Models {
    DECLARE_JSON_STRUCT(BssbLevel) {
        VALUE(std::string, LevelId);
        VALUE(std::string, SongName);
        VALUE(std::string, SongSubName);
        VALUE(std::string, SongAuthorName);

        /// HTTP URL for the cover art associated with the level, if any.
        /// Provided by the API when querying lobbies.
        VALUE(std::string, CoverArtUrl);


        __declspec(property(get = get_ListDescription)) std::string ListDescription;
        std::string get_ListDescription() {
                if (SongName.empty())
                    return "Unknown";
                                        
                std::string text;
                if (!SongAuthorName.empty())
                {
                    text += SongAuthorName;
                    text += " - ";
                }
                text += SongName;
                return text;
        }
    };
}