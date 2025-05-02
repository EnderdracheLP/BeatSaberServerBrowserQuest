#pragma once

#include "rapidjson-macros/shared/macros.hpp"

namespace ServerBrowser::Models::Responses {
    DECLARE_JSON_STRUCT(AnnounceResultsData) {
        VALUE(bool, Success);
        VALUE(std::string, Key);
        NAMED_VALUE(std::string, ServerMessage, "Message");
    };
}