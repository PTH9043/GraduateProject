#include "CoreDefines.h"
#include "UAwsConnector.h"

namespace Core{

	UAwsConnector::UAwsConnector(OBJCON_CONSTRUCTOR) :
		UObject(OBJCON_CONDATA), m_isAwsActivated{false}, m_CheckTerminationCount{0}
	{
	}

	_bool UAwsConnector::NativeConstruct()
	{
		try
		{

		}
		catch (_int exception) {

		}
		return true;
	}

	void UAwsConnector::OnStartGameSession(Aws::GameLift::Server::Model::GameSession _Session)
	{
	}

	void UAwsConnector::ProcessTerminate()
	{
	}

	void UAwsConnector::Free()
	{
	}

}