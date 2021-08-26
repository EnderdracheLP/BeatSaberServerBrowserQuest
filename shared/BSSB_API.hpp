#pragma once

#include "API/ServerBrowser/Page.hpp"

#include <string>

namespace ServerBrowser::API {

    //std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder = "Relevance", std::string ME = "", std::string NE = "", std::string Chroma = "");

    std::optional<ServerBrowser::API::Page> Browse(std::string platform = "oculus", int filterFull = 0, int vanilla = 0);

    //std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap);

    //void GetBeatmapByHashAsync(std::string hash, std::function<void(std::optional<BeatSaver::Beatmap>)> finished);

    //void SearchPagedAsync(std::string query, int pageIndex, std::function<void(std::optional<BeatSaver::Page>)> finished, std::string sortOrder = "Relevance", std::string automapper = "", std::string ranked = "", std::string ME = "", std::string NE = "", std::string Chroma = "");

    void BrowseAsync(std::function<void(std::optional<ServerBrowser::API::Page>)> finished, std::string platform = "oculus", int filterFull = 0, int vanilla = 0);

    //void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr);

    extern std::string exception;

}