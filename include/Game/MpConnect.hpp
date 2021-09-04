#pragma once
#include "main.hpp"
#include "Core/HostedGameData.hpp"
#include "GlobalNamespace/MasterServerEndPoint.hpp"
using GlobalNamespace::MasterServerEndPoint;
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
	//	static MasterServerEndPoint* _officialEndPoint;
	//	static MasterServerEndPoint* _moddedEndPoint;
	//	static bool _usingModdedServer;
		static MasterServerEndPoint* OverrideEndPoint;
		//static bool ShouldDisableCertificateValidation;
	public:
		static MasterServerEndPoint* get_OverrideEndPoint();

	static MasterServerEndPoint* LastUsedMasterServer;
	//	MasterServerEndPoint* get_LastUsedMasterServer();
	//private:
	//	void set_OverrideEndPoint(MasterServerEndPoint* NewOverrideEndPoint);
	//	void set_LastUsedMasterServer(MasterServerEndPoint* LastUsedEndPoint);

	//public:
	static bool get_ShouldDisableCertificateValidation();

	static void ReportCurrentMasterServerValue(MasterServerEndPoint* currentEndPoint);

	static void SetMasterServerOverride(std::string hostName, int port = DEFAULT_MASTER_PORT);

	static void SetMasterServerOverride(MasterServerEndPoint* overrideEndPoint);

	static void ClearMasterServerOverride();
#pragma endregion
	};
}

