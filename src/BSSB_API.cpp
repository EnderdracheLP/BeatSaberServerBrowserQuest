#include "main.hpp"
#include "BSSB_API.hpp"

#include "Utils/WebUtils.hpp"

#include "Exceptions.hpp"

#define BASE_URL std::string("https://bssb.app/api/v1")
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ServerBrowser::API {

    std::string exception;

    //std::optional<ServerBrowser::Lobby> GetBeatmapByKey(std::string key) {
    //    auto json = WebUtils::GetJSON(API_URL + "/maps/id/" + key);
    //    if (!json.has_value())
    //        return std::nullopt;
    //    try {
    //        BeatSaver::Beatmap beatmap;
    //        beatmap.Deserialize(json.value().GetObject());
    //        if (!exception.empty()) exception.clear();
    //        return beatmap;
    //    }
    //    catch (const SongDownloader::JsonException& e) {
    //        LOG_ERROR("%s", e.what());
    //        exception = e.what();
    //        return std::nullopt;
    //    }
    //}

    //std::optional<BeatSaver::Beatmap> GetBeatmapByHash(std::string hash) {
    //    auto json = WebUtils::GetJSON(API_URL + "/maps/hash/" + hash);
    //    if (!json.has_value())
    //        return std::nullopt;
    //    try {
    //        BeatSaver::Beatmap beatmap;
    //        beatmap.Deserialize(json.value().GetObject());
    //        if (!exception.empty()) exception.clear();
    //        return beatmap;
    //    }
    //    catch (const SongDownloader::JsonException& e) {
    //        LOG_ERROR("%s", e.what());
    //        exception = e.what();
    //        return std::nullopt;
    //    }
    //}

    //std::optional<BeatSaver::UserDetail> GetUserById(int id) {
    //    auto json = WebUtils::GetJSON(API_URL + "/users/id/" + std::to_string(id));
    //    if (!json.has_value())
    //        return std::nullopt;
    //    try {
    //        BeatSaver::UserDetail user;
    //        user.Deserialize(json.value().GetObject());
    //        if (!exception.empty()) exception.clear();
    //        return user;
    //    }
    //    catch (const SongDownloader::JsonException& e) {
    //        LOG_ERROR("%s", e.what());
    //        exception = e.what();
    //        return std::nullopt;
    //    }
    //}

    //std::optional<BeatSaver::Page> SearchPaged(std::string query, int pageIndex, std::string sortOrder, std::string automapper, std::string ME, std::string NE, std::string Chroma) {
    //    std::string searchPath = API_URL + "/search/text/" + std::to_string(pageIndex) + "?sortOrder=" + sortOrder;
    //    if (!automapper.empty()) searchPath += "&automapper=" + automapper;
    //    if (!query.empty()) searchPath += "&q=" + query;
    //    if (!ME.empty()) searchPath += "&me=" + ME;
    //    if (!NE.empty()) searchPath += "&noodle=" + NE;
    //    if (!Chroma.empty()) searchPath += "&chroma=" + Chroma;
    //    auto json = WebUtils::GetJSON(searchPath);
    //    if (!json.has_value())
    //        return std::nullopt;
    //    try {
    //        BeatSaver::Page page;
    //        page.Deserialize(json.value().GetObject());
    //        if (!exception.empty()) exception.clear();
    //        return page;
    //    }
    //    catch (const SongDownloader::JsonException& e) {
    //        LOG_ERROR("%s", e.what());
    //        exception = e.what();
    //        return std::nullopt;
    //    }
    //}

    std::optional<ServerBrowser::API::Page> Browse(std::string platform, int filterFull, int vanilla) {
        std::string browsePath = BASE_URL + "/browse?platform=" + platform;
        if (filterFull > 0) browsePath += "&filterFull=" + std::to_string(filterFull);
        if (vanilla > 0) browsePath += "&vanilla=" + std::to_string(vanilla);
        std::optional<rapidjson::Document> json;
        json = WebUtils::GetJSON(browsePath);
        if (!json.has_value())
            return std::nullopt;
        try {
            ServerBrowser::API::Page page;
            page.Deserialize(json.value().GetObject());
            if (!exception.empty()) exception.clear();
            return page;
        }
        catch (const std::exception& e) {
            getLogger().error("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    //std::optional<BeatSaver::Page> PlaysPaged(int pageIndex) {
    //    auto json = WebUtils::GetJSON(API_URL + "/maps/plays/" + std::to_string(pageIndex));
    //    if (!json.has_value())
    //        return std::nullopt;
    //    try {
    //        BeatSaver::Page page;
    //        page.Deserialize(json.value().GetObject());
    //        if (!exception.empty()) exception.clear();
    //        return page;
    //    }
    //    catch (const SongDownloader::JsonException& e) {
    //        LOG_ERROR("%s", e.what());
    //        exception = e.what();
    //        return std::nullopt;
    //    }
    //}

    //bool DownloadBeatmap(const BeatSaver::Beatmap& beatmap) {
    //    auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + beatmap.GetId() + " ()[]{}%&.:,;=!-_ (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")";
    //    std::string data;
    //    WebUtils::Get(beatmap.GetVersions().front().GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT, data);
    //    int args = 2;
    //    int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
    //        return 0;
    //        }, &args);
    //    return statusCode;
    //}

    //std::vector<uint8_t> GetPreview(const BeatSaver::Beatmap& beatmap) {
    //    std::string data;
    //    WebUtils::Get(beatmap.GetVersions().front().GetPreviewURL(), FILE_DOWNLOAD_TIMEOUT, data);
    //    std::vector<uint8_t> bytes(data.begin(), data.end());
    //    return bytes;
    //}

    //std::vector<uint8_t> GetCoverImage(const BeatSaver::Beatmap& beatmap) {
    //    std::string data;
    //    WebUtils::Get(beatmap.GetVersions().front().GetCoverURL(), FILE_DOWNLOAD_TIMEOUT, data);
    //    std::vector<uint8_t> bytes(data.begin(), data.end());
    //    return bytes;
    //}

    //// This isn't really ideal, but it could be kinda useful to be able to get the coverImage just by hash directly
    //std::optional<std::vector<uint8_t>> GetCoverImage(std::string hash) {
    //    std::string data;
    //    WebUtils::Get(CDN_URL + hash + ".jpg", FILE_DOWNLOAD_TIMEOUT, data);
    //    if (data.empty()) return std::nullopt;
    //    std::vector<uint8_t> bytes(data.begin(), data.end());
    //    return bytes;
    //}

    void BrowseAsync(std::function<void(std::optional<ServerBrowser::API::Page>)> finished, std::string platform, int filterFull, int vanilla) {
        exception.clear();
        std::string browsePath = BASE_URL + "/browse?platform=" + platform;
        if (filterFull > 0) browsePath += "&filterFull=" + std::to_string(filterFull);
        if (vanilla > 0) browsePath += "&vanilla=" + std::to_string(vanilla);
        WebUtils::GetJSONAsync(browsePath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ServerBrowser::API::Page page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        getLogger().error("%s", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        // Convert the document into a string and log/write to file for debug purposes
                        rapidjson::StringBuffer buffer;
                        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        document.Accept(writer);
                        writefile("/sdcard/ModData/ServerBrowser_Browse.json", buffer.GetString());
                    }
                }
            }
        );
    }

    //void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
    //    WebUtils::GetAsync(beatmap.GetVersions().front().GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT,
    //        [beatmap, finished](long httpCode, std::string data) {
    //            auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + FileUtils::FixIlegalName(beatmap.GetId() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")");
    //            int args = 2;
    //            int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
    //                return 0;
    //                }, &args);
    //            finished(statusCode);
    //        }, progressUpdate
    //    );
    //}

    //void GetCoverImageAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
    //    WebUtils::GetAsync(beatmap.GetVersions().front().GetCoverURL(), FILE_DOWNLOAD_TIMEOUT,
    //        [beatmap, finished](long httpCode, std::string data) {
    //            std::vector<uint8_t> bytes(data.begin(), data.end());
    //            finished(bytes);
    //        }, progressUpdate
    //    );
    //}

    //void GetCoverImageByHashAsync(std::string hash, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
    //    WebUtils::GetAsync(CDN_URL + hash + ".jpg", FILE_DOWNLOAD_TIMEOUT,
    //        [hash, finished](long httpCode, std::string data) {
    //            LOG_DEBUG("HTTP Code when loading by hash: %ld", httpCode);
    //            if (httpCode != 0) return;
    //            std::vector<uint8_t> bytes(data.begin(), data.end());
    //            finished(bytes);
    //        }, progressUpdate
    //    );
    //}

    //void GetPreviewAsync(const BeatSaver::Beatmap& beatmap, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
    //    WebUtils::GetAsync(beatmap.GetVersions().front().GetPreviewURL(), FILE_DOWNLOAD_TIMEOUT,
    //        [beatmap, finished](long httpCode, std::string data) {
    //            std::vector<uint8_t> bytes(data.begin(), data.end());
    //            finished(bytes);
    //        }, progressUpdate
    //    );
    //}


}