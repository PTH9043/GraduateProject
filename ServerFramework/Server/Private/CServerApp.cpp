#include "ServerDefines.h"
#include "CServerApp.h"
#include "UCoreInstance.h"
#include "CServerAdiminstor.h"


namespace Server
{
	CServerApp::CServerApp() 
	{

	}

	_bool CServerApp::Start()
	{
		CreateServiceObject<CServerAdiminstor>();
		std::cout << "Start Server!\n";
		return GetService()->Start();
	}
}