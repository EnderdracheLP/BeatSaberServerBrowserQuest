#include <string>

namespace ServerBrowser::Core {
    /// <summary>
    /// Tracks the state of announces sent to the BSSB API.
    /// </summary>
    struct AnnounceState
    {
        std::string ServerCode = "";
        std::string OwnerId = "";
        std::string HostSecret = "";

        bool DidAnnounce = false;
        int LastSuccess = 0;

        bool DidFail = false;
        int LastFailure = 0;
    };
}