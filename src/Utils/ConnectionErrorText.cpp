#include "Utils/ConnectionErrorText.hpp"
using GlobalNamespace::ConnectionFailedReason;

namespace ServerBrowser::Utils {
    const std::string ConnectionErrorText::Generate(GlobalNamespace::ConnectionFailedReason reason) {
        int reasonInt = reason.value;
        //std::string reasonStr = to_utf8(csstrtostr(reinterpret_cast<System::Enum*>(&reason)->ToString()));
        //getLogger().debug("Reason: %s", reasonStr.c_str());
        //std::string reasonStr = to_utf8(csstrtostr(System::Enum::GetName(csTypeOf(GlobalNamespace::ConnectionFailedReason), System::Enum::ToObject(reason.value))));
        //std::string msg = string_format("Error CFR-%d (%s)", reasonInt, reasonStr.c_str());
        std::string msg = string_format("Error CFR-%d", reasonInt);
        
        switch (reason)
        {
        case ConnectionFailedReason::ServerUnreachable:
            msg += " (Server Unreachable).\r\nCould not connect to the game host.";
            break;
        case ConnectionFailedReason::ServerIsTerminating:
            msg += " (Server Is Terminating).\r\nIt looks like this game has already ended.";
            break;
        case ConnectionFailedReason::ServerDoesNotExist:
            msg += " (Server Does Not Exist).\r\nIt looks like this game has already ended.";
            break;
        case ConnectionFailedReason::ServerAtCapacity:
            msg += " (Server At Capacity).\r\nThis game is full.";
            break;
        case ConnectionFailedReason::NetworkNotConnected:
            msg += " (Network Not Connected).\r\nCould not connect to master server, check your connection and try again.";
            break;
        case ConnectionFailedReason::MasterServerUnreachable:
            msg += " (MasterServer Unreachable).\r\nCould not connect to master server, check your connection and try again.";
            break;
        case ConnectionFailedReason::VersionMismatch:
            msg += " (Version Mismatch).\r\nMake sure you and the host are using the same game version.";
            break;
        case ConnectionFailedReason::InvalidPassword:
            msg += " (Invalid Password).";
            break;
        case ConnectionFailedReason::MasterServerNotAuthenticated:
            msg += " (MasterServer Not Authenticated).\r\nCould not authenticate master server, you may need to restart the game.";
            break;
        case ConnectionFailedReason::MasterServerCertificateValidationFailed:
            msg += " (MasterServer Certificate Validation Failed).\r\nCould not authenticate master server, you may need to restart the game.";
            break;
        default:
            msg += " (Unknown).\r\nUnknown error occurred, check your connection and try again.";
            break;
        }
        return msg;
    }
}