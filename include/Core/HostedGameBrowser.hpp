#include "Core/ServerBrowserResult.hpp"
#include "Core/HostedGameData.hpp"
#include <vector>
#include <map>

namespace ServerBrowser::Core {
    class HostedGameBrowser {
        static std::optional<ServerBrowserResult> lastServerResult;
        static std::map<int, HostedGameData> lobbyObjects;
        static std::vector<HostedGameData> lobbiesOnPage;
        static int offset;
        //static void LoadPageFunc(int pageOffset, std::optional<ServerBrowserResult> result);
    public:
        static std::function<void()> OnUpdate;

        static void FullRefresh(/*HostedGameFilters filters*/);
        static void LoadPage(int pageOffset/*, HostedGameFilters filters*/);
        static bool get_ConnectionOk();
        static bool get_AnyResultsOnPage();
        static int get_TotalResultCount();
        static int get_PageIndex();
        static int get_TotalPageCount();
        static int get_PageSize();
        static std::vector<HostedGameData>& get_LobbiesOnPage();
        static std::string get_ServerMessage();
    };
}