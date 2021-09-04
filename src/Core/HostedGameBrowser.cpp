//TODO: Remove temp main.hpp include
#include "main.hpp"

#include "Core/HostedGameBrowser.hpp"
#include "Core/BSSBMasterAPI.hpp"
#include <cmath>

namespace ServerBrowser::Core {
    std::function<void()> HostedGameBrowser::OnUpdate = nullptr;

    std::optional<ServerBrowserResult> HostedGameBrowser::lastServerResult;
    std::map<int, HostedGameData> HostedGameBrowser::lobbyObjects;
    std::vector<HostedGameData> HostedGameBrowser::lobbiesOnPage;
    int HostedGameBrowser::offset;

    void HostedGameBrowser::FullRefresh(/*HostedGameFilters filters*/) {
        lastServerResult = std::nullopt;
        lobbyObjects.clear();
        lobbiesOnPage.clear();

        LoadPage(0/*, filters*/);
    }
    void HostedGameBrowser::LoadPage(int pageOffset/*, HostedGameFilters filters*/) {
        getLogger().debug("Check pageOffset is: %d", pageOffset);
        // Send API Request
        BSSBMasterAPI::BrowseAsync(pageOffset, /*filters,*/
            [&, pageOffset](std::optional<ServerBrowserResult> result) {
                // Update state
                if (result.has_value()) {
                    offset = pageOffset;
                    getLogger().debug("offset is now: %d", offset);
                    lastServerResult = result;

                    // If we got results, process and index the lobby info
                    std::vector<HostedGameData> nextLobbiesOnPage;

                    for (HostedGameData lobby : lastServerResult->Getlobbies()) {
                        getLogger().debug("Adding Lobby with Id: %d", lobby.get_Id());
                        lobbyObjects.insert_or_assign(lobby.get_Id(), lobby);
                        nextLobbiesOnPage.push_back(lobby);
                    }

                    lobbiesOnPage = nextLobbiesOnPage;
                }

                // Fire update event for the UI
                if (OnUpdate) OnUpdate();
            });
    }
    bool HostedGameBrowser::get_ConnectionOk() {
        return lastServerResult.has_value();
    }
    bool HostedGameBrowser::get_AnyResultsOnPage() {
        return lastServerResult.has_value() && !lastServerResult->Getlobbies().empty();
    }
    int HostedGameBrowser::get_TotalResultCount() {
        return lastServerResult.has_value() ? lastServerResult->Getcount() : 0;
    }
    int HostedGameBrowser::get_PageIndex() {
        getLogger().debug("offset is: %d", offset);
        getLogger().debug("PageSize is: %d", get_PageSize());
        int result = floor(offset / get_PageSize());
        getLogger().debug("Result is: %d", result);
        return result;
    }
    int HostedGameBrowser::get_TotalPageCount() {
        return (int)ceil((double)get_TotalResultCount() / (double)get_PageSize());
    }
    int HostedGameBrowser::get_PageSize() {
        return lastServerResult.has_value() ? lastServerResult->Getlimit() : 10;
    }
    std::vector<HostedGameData>& HostedGameBrowser::get_LobbiesOnPage() {
        return lobbiesOnPage;
    }
    std::string HostedGameBrowser::get_ServerMessage() {
        return lastServerResult.has_value() ? lastServerResult->Getmessage() : "";
    }
}