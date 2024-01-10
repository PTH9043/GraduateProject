#include "CoreDefines.h"
#include "UAwsConnector.h"
#include "UCoreInstance.h"

namespace Core{

	UAwsConnector::UAwsConnector(OBJCON_CONSTRUCTOR) :
		UObject(OBJCON_CONDATA), m_isAwsActivated{false}, 
		m_CheckTerminationCount{0}, m_AwsGameSession{}, m_SDKOptions{}
	{
	}

	_bool UAwsConnector::Start()
	{
		SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
		try
		{
			Aws::InitAPI(m_SDKOptions);

			const auto& InitOutCome = Aws::GameLift::Server::InitSDK();
			if (false == InitOutCome.IsSuccess())
			{
				return false;
			}
			VECTOR<std::string> LogPaths{ "..\\Bin\\AwsServerLog.txt" };
			auto processReadyParameter = Aws::GameLift::Server::ProcessParameters(
				std::bind(&UAwsConnector::OnStartGameSession, this, std::placeholders::_1),
				std::bind(&UAwsConnector::OnProcessTerminate, this),
				std::bind(&UAwsConnector::IsAwsActivated, this),
				TCP_PORT_NUM, Aws::GameLift::Server::LogParameters(LogPaths)
			); 

			const auto& ReadyOutCome = Aws::GameLift::Server::ProcessReady(processReadyParameter);

			if (!ReadyOutCome.IsSuccess())
			{
				return false;
			}

			m_isAwsActivated = true;

			LOGPRINTOUT_CLASSLINE(spCoreInstance, "GameLift: Process Ready Success");
		}
		catch (_int exception) {
			LOGPRINTOUT_CLASSLINE(spCoreInstance, "Failed GameLift");
			LOGPRINTOUT(spCoreInstance, "Exception Code: %d", exception);
			CRASH("");
		}
		return true;
	}

	void UAwsConnector::OnStartGameSession(Aws::GameLift::Server::Model::GameSession _Session)
	{
		SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
		// Active Gamesession
		Aws::GameLift::Server::ActivateGameSession();

		m_AwsGameSession = _Session;

		StartGameSession();
		LOGPRINTOUT_CLASSLINE(spCoreInstance, "OnStartGameSession Success");
	}

	void UAwsConnector::OnProcessTerminate()
	{
		if (true == m_isAwsActivated)
		{
			SHPTR<UCoreInstance> spCoreInstance = GetCoreInstance();
			LOGPRINTOUT_CLASSLINE(spCoreInstance, "OnProcessTerminate Success");
			TerminateGameSession(0xDEAD);
		}
	}

	void UAwsConnector::StartGameSession()
	{
	}

	void UAwsConnector::TerminateGameSession(_int _exitCode)
	{
		Aws::GameLift::Server::ProcessEnding();

		m_isAwsActivated = false;

		std::exit(_exitCode);
	}

	void UAwsConnector::Free()
	{
		Aws::GameLift::Server::Destroy();
		Aws::ShutdownAPI(m_SDKOptions);
	}
}