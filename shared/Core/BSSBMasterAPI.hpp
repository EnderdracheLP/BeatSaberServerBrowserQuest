#pragma once

#include "ServerBrowserResult.hpp"

#include <string>

namespace ServerBrowser::Core {
    class BSSBMasterAPI {
        static std::string exception;
    public:
        static const std::string get_Exception() { return exception; }
        //static std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder = "Relevance", std::string ME = "", std::string NE = "", std::string Chroma = "");

        static std::optional<ServerBrowser::Core::ServerBrowserResult> Browse(int offset/*, HostedGameFilters filters*/);

        static std::vector<uint8_t> GetCoverImage(const ServerBrowser::Core::HostedGameData& lobby);

        //static void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

        //static void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished, std::string sortOrder = "Relevance", std::string automapper = "", std::string ranked = "", std::string ME = "", std::string NE = "", std::string Chroma = "");

        static void BrowseAsync(int offset, /*HostedGameFilters filters,*/ std::function<void(std::optional<ServerBrowser::Core::ServerBrowserResult>)> finished);

        static void GetCoverImageAsync(const ServerBrowser::Core::HostedGameData& lobby, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);
    };
}