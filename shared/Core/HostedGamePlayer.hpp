#pragma once
#include <string>

namespace ServerBrowser::Core {
    struct HostedGamePlayer {
        int SortIndex;
        std::string UserId;
        std::string UserName;
        bool IsHost;
        bool IsAnnouncer;
        float Latency;
    };
}