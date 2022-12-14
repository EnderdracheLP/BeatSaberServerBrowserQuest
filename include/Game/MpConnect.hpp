#pragma once
#include "main.hpp"
#include "Core/HostedGameData.hpp"
#include "GlobalNamespace/DnsEndPoint.hpp"
using GlobalNamespace::DnsEndPoint;
using ServerBrowser::Core::HostedGameData;

namespace ServerBrowser::Game {
	class MpConnect
	{
	public:

		#define OFFICIAL_MASTER_SUFFIX ".mp.beatsaber.com"

		#define OFFICIAL_MASTER_OCULUS "oculus.production.mp.beatsaber.com"
		#define OFFICIAL_MASTER_TEST "staging.mp.beatsaber.com"
		
		#define DEFAULT_MASTER_PORT 2328

		static void Join(HostedGameData game);
		
#pragma region Master Server Management
	// TODO: Implement this possibly
	private:
	//	static DnsEndPoint* _officialEndPoint;
	//	static DnsEndPoint* _moddedEndPoint;
	//	static bool _usingModdedServer;
	static SafePtr<DnsEndPoint> OverrideEndPoint;
	//static std::pair<std::string, int> OverrideEndPoint;
		//static bool ShouldDisableCertificateValidation;
	public:
		static DnsEndPoint* const get_OverrideEndPoint();

	static SafePtr<DnsEndPoint> LastUsedMasterServer;
	//static std::pair<std::string, int> LastUsedMasterServer;
	//	DnsEndPoint* get_LastUsedMasterServer();
	//private:
	//	void set_OverrideEndPoint(DnsEndPoint* NewOverrideEndPoint);
	//	void set_LastUsedMasterServer(DnsEndPoint* LastUsedEndPoint);

	//public:
	static bool get_ShouldDisableCertificateValidation();

	static void ReportCurrentMasterServerValue(DnsEndPoint* currentEndPoint);

	static void SetMasterServerOverride(std::string hostName, int port = DEFAULT_MASTER_PORT);

	static void SetMasterServerOverride(DnsEndPoint* overrideEndPoint);

	static void ClearMasterServerOverride();
#pragma endregion
	};
}

