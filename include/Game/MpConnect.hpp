#pragma once
#include "main.hpp"
#include "Core/HostedGameData.hpp"
#include "GlobalNamespace/DnsEndPoint.hpp"
// #include "GlobalNamespace/MasterServerEndPoint.hpp"
// using GlobalNamespace::MasterServerEndPoint;
// using ServerBrowser::Core::HostedGameData;

namespace ServerBrowser::Game {
	class MpConnect
	{
	public:

		#define OFFICIAL_MASTER_SUFFIX ".mp.beatsaber.com"

		#define OFFICIAL_MASTER_OCULUS "oculus.production.mp.beatsaber.com"
		#define OFFICIAL_MASTER_TEST "staging.mp.beatsaber.com"
		
		#define DEFAULT_MASTER_PORT 2328

		static void Join(ServerBrowser::Core::HostedGameData game);
		
#pragma region Master Server Management
	// TODO: Implement this possibly
	private:
	//	static MasterServerEndPoint* _officialEndPoint;
	//	static MasterServerEndPoint* _moddedEndPoint;
	//	static bool _usingModdedServer;
	static SafePtr<GlobalNamespace::DnsEndPoint> OverrideEndPoint;
	//static std::pair<std::string, int> OverrideEndPoint;
		//static bool ShouldDisableCertificateValidation;
	public:
		static GlobalNamespace::DnsEndPoint* const get_OverrideEndPoint();

	static SafePtr<GlobalNamespace::DnsEndPoint> LastUsedMasterServer;
	//static std::pair<std::string, int> LastUsedMasterServer;
	//	MasterServerEndPoint* get_LastUsedMasterServer();
	//private:
	//	void set_OverrideEndPoint(MasterServerEndPoint* NewOverrideEndPoint);
	//	void set_LastUsedMasterServer(MasterServerEndPoint* LastUsedEndPoint);

	//public:
	static bool get_ShouldDisableCertificateValidation();

	static void ReportCurrentMasterServerValue(GlobalNamespace::DnsEndPoint* currentEndPoint);

	static void SetMasterServerOverride(std::string hostName, int port = DEFAULT_MASTER_PORT);

	static void SetMasterServerOverride(GlobalNamespace::DnsEndPoint* overrideEndPoint);

	static void ClearMasterServerOverride();
#pragma endregion
	};
}

