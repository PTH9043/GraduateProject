#include "ServerDefines.h"
#include "CAwsServerConnector.h"

namespace Server
{
	CAwsServerConnector::CAwsServerConnector(OBJCON_CONSTRUCTOR)
		: UAwsConnector(OBJCON_CONDATA)
	{
	}

	_bool CAwsServerConnector::Start()
	{
		std::cout << "AWS Start\n";
		return __super::Start();
	}

	void CAwsServerConnector::StartGameSession()
	{
		/* AWS가 시작될 때 */

	}

	void CAwsServerConnector::TerminateGameSession(_int _exitCode)
	{
		/* 해당 AWS가 종료될 때 */
		__super::TerminateGameSession(_exitCode);
	}

	void CAwsServerConnector::Free()
	{
	}
}