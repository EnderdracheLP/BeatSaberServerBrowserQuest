#include "main.hpp"
#include "GlobalNamespace/DnsEndPoint.hpp"

namespace ServerBrowser::Core {
    struct DnsEndPointW {
        StringW hostName;
        int port;
        constexpr DnsEndPointW() noexcept {
            this->Clear();
        }
        // constexpr DnsEndPointW() noexcept : _dnsEndPoint(nullptr) {}
        constexpr operator GlobalNamespace::DnsEndPoint*() const {
            return GlobalNamespace::DnsEndPoint::New_ctor(hostName, port);
            // return _dnsEndPoint;
        }
        constexpr GlobalNamespace::DnsEndPoint* operator->() noexcept {
            return GlobalNamespace::DnsEndPoint::New_ctor(hostName, port);
            // return _dnsEndPoint;
        }
        constexpr operator bool() const noexcept {
            return hostName && port > 0;
        }
        // inline DnsEndPointW& operator=(GlobalNamespace::DnsEndPoint* other) {
        //     this->~DnsEndPointW();
        //     _dnsEndPoint = other;
        //     return *this;
        // }
        // inline DnsEndPointW& operator=(DnsEndPointW& other) {
        //     this->~DnsEndPointW();
        //     return *this;
        // }
        StringW ToString() const {
            return StringW(string_format("%s:%d", static_cast<std::string>(hostName).c_str(), port));
        }
        std::string ToCPPString() const {
            return string_format("%s:%d", static_cast<std::string>(hostName).c_str(), port);
        }
        void Clear() {
            hostName = nullptr;
            port = 0;
        }
        bool operator ==(DnsEndPointW rhs) const noexcept {
            return ToCPPString() == rhs.ToCPPString();
        }
        bool operator ==(GlobalNamespace::DnsEndPoint* rhs) const noexcept {
            return ToCPPString() == static_cast<std::string>(rhs->ToString());
        }
        constexpr DnsEndPointW(StringW host, int port) : hostName(host), port(port) {
            // _dnsEndPoint = GlobalNamespace::DnsEndPoint::New_ctor<il2cpp_utils::CreationType::Manual>(host, port);
        }
        constexpr DnsEndPointW(GlobalNamespace::DnsEndPoint* dnsEndPoint) : hostName(dnsEndPoint->hostName), port(dnsEndPoint->port) {
            // _dnsEndPoint = GlobalNamespace::DnsEndPoint::New_ctor<il2cpp_utils::CreationType::Manual>(dnsEndPoint->hostName, dnsEndPoint->port);
        }
        constexpr ~DnsEndPointW() {
            // gc_free_specific(_dnsEndPoint);
        }
    // private:
    //     GlobalNamespace::DnsEndPoint* _dnsEndPoint;
    };
}