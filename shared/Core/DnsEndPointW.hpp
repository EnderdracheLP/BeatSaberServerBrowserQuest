#include "main.hpp"
#include "GlobalNamespace/DnsEndPoint.hpp"

namespace ServerBrowser::Core {
    struct DnsEndPointW {
        StringW hostName;
        int port;
        GlobalNamespace::DnsEndPoint* _dnsEndPoint;
        constexpr DnsEndPointW() noexcept {
            this->Clear();
        }
        // constexpr DnsEndPointW() noexcept : _dnsEndPoint(nullptr) {}
        constexpr operator GlobalNamespace::DnsEndPoint*() const {
            return _dnsEndPoint;
        }
        constexpr GlobalNamespace::DnsEndPoint* operator->() noexcept {
            return _dnsEndPoint;
        }
        constexpr operator bool() const noexcept {
            return hostName && port > 0;
        }
        inline DnsEndPointW& operator=(GlobalNamespace::DnsEndPoint* other) {
            if (other != nullptr && other->hostName && other->port != 0 &&
                (other->hostName != this->hostName || other->port != this->port))
            {
                this->~DnsEndPointW();
                // _dnsEndPoint = other;
                this->hostName = other->hostName;
                this->port = other->port;
                // this->_dnsEndPoint = GlobalNamespace::DnsEndPoint::New_ctor<il2cpp_utils::CreationType::Manual>(other->hostName, other->port);
            }
            return *this;
        }
        inline DnsEndPointW& operator=(DnsEndPointW& other) {
            this->~DnsEndPointW();
            this->hostName = other.hostName;
            this->port = other.port;
            this->_dnsEndPoint = other._dnsEndPoint;
            return *this;
        }
        StringW ToString() const {
            return StringW(this->ToCPPString());
        }
        std::string ToCPPString() const {
            return string_format("%s:%d", static_cast<std::string>(hostName).c_str(), port);
        }
        operator std::string() const {
            return ToCPPString();
        }
        void Clear() {
            if (_dnsEndPoint) gc_free_specific(_dnsEndPoint);
            _dnsEndPoint = nullptr;
            hostName = nullptr;
            port = 0;
        }
        bool operator ==(DnsEndPointW rhs) const noexcept {
            return ToCPPString() == rhs.ToCPPString();
        }
        bool operator ==(GlobalNamespace::DnsEndPoint* rhs) const noexcept {
            return ToCPPString() == static_cast<std::string>(rhs->ToString());
        }
        void InitDnsEndpoint() {
            if (il2cpp_functions::initialized) {
                if (!classof(GlobalNamespace::DnsEndPoint*)->initialized) il2cpp_functions::Class_Init(classof(GlobalNamespace::DnsEndPoint*)); // This is needed in order to initialize the Il2CppClass
                this->_dnsEndPoint = GlobalNamespace::DnsEndPoint::New_ctor<il2cpp_utils::CreationType::Manual>(hostName, port);
            }
        }
        constexpr DnsEndPointW(StringW host, int port) : hostName(host), port(port) {
            InitDnsEndpoint();
        }
        constexpr DnsEndPointW(GlobalNamespace::DnsEndPoint* dnsEndPoint) : hostName(dnsEndPoint->hostName), port(dnsEndPoint->port) {
            InitDnsEndpoint();
        }
        constexpr ~DnsEndPointW() {
            Clear();
        }
    // private:
    //     GlobalNamespace::DnsEndPoint* _dnsEndPoint;
    };
}