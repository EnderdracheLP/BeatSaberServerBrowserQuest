#include "Utils/ConnectionErrorText.hpp"
#include "Utils/EnumUtils.hpp"
using GlobalNamespace::ConnectionFailedReason;

namespace ServerBrowser::Utils {

    // Function to add spaces before capitalized letters
    std::string addSpaceBeforeCapLetter(std::string str)
    {
        std::string result;
        // Traverse the string
        for(int i=0; i < str.length(); i++)
        {
            // Find all uppercase letters
            if (str[i]>='A' && str[i]<='Z')
            { 
                // Print space before it
                // if its an uppercase character
                if (i != 0)
                    result += " ";
            }
            // Add result to string
            result += str[i];
        }
        return result;
    }

    StringW ConnectionErrorText::Generate(GlobalNamespace::ConnectionFailedReason reason) {
        int reasonInt = reason.value;
        //std::string reasonStr = to_utf8(csstrtostr(reinterpret_cast<System::Enum*>(&reason)->ToString()));
        //getLogger().debug("Reason: %s", reasonStr.c_str());
        //std::string reasonStr = to_utf8(csstrtostr(System::Enum::GetName(csTypeOf(GlobalNamespace::ConnectionFailedReason), System::Enum::ToObject(reason.value))));
        //std::string msg = string_format("Error CFR-%d (%s)", reasonInt, reasonStr.c_str());
        StringW msg = string_format("Error CFR-%d", reasonInt).c_str();
        msg += " (" + addSpaceBeforeCapLetter(EnumUtils::GetEnumName(reason)) + ")";
        switch (reason)
        {
            default:
            case ConnectionFailedReason::None: // CFR-0
            case ConnectionFailedReason::Unknown: // CFR-1
                msg += "\r\nAn unknown error occurred. Please try again.";
                break;
            
            case ConnectionFailedReason::ConnectionCanceled: // CFR-2
                // user should not normally see this
                msg += "\r\nYou cancelled the connection."; 
                break;
            
            case ConnectionFailedReason::ServerUnreachable: // CFR-3
                msg += "\r\nCould not connect to the game server.";
                break;
            
            case ConnectionFailedReason::ServerAlreadyExists: // CFR-4
                // user should not normally see this
                msg += "\r\nThe server already exists (this should never happen).";
                break;
                
            case ConnectionFailedReason::ServerDoesNotExist: // CFR-5
                msg += "\r\nThe server code is invalid, or the lobby already ended.";
                break;
                
            case ConnectionFailedReason::ServerAtCapacity: // CFR-6
                msg += "\r\nThe server is already full.";
                break;
                
            case ConnectionFailedReason::VersionMismatch: // CFR-7
                // user should not normally see this
                msg += "\r\nYour game version is not compatible with this server.";
                break;
                
            case ConnectionFailedReason::InvalidPassword: // CFR-8
                // user should not normally see this
                msg += "\r\nPlease enter the correct password to connect to this server.";
                break;
            
            case ConnectionFailedReason::MultiplayerApiUnreachable: // CFR-9
                // this is now used for both GameLift API failures and general master server connect fails
                msg += "\r\nCould not connect to master server.";
                break;
            
            case ConnectionFailedReason::AuthenticationFailed: // CFR-10
                msg += "\r\nLog in failed. You may need to restart your game and/or Steam.";
                break;
            
            case ConnectionFailedReason::NetworkNotConnected: // CFR-11
                msg += "\r\nNetwork error. Please check your connection and try again.";
                break;
                
            case ConnectionFailedReason::CertificateValidationFailed: // CFR-12
                msg += "\r\nThe master server's certificate could not be validated.";
                break;
                
            case ConnectionFailedReason::ServerIsTerminating: // CFR-13
                msg += "\r\nThis game server is already shutting down.";
                break;
            
            case ConnectionFailedReason::Timeout: // CFR-14
                msg += "\r\nThe connection to the server timed out.";
                break;
            
            case ConnectionFailedReason::FailedToFindMatch: // CFR-15
                msg += "\r\nMatchmaking failed. Please try again.";
                break;
        }
        return msg;
    }
}