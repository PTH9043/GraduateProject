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
		std::cout << CurrentMilliseconds() << "\n";
		std::cout << "Start Server!\n";
		try
		{
			RegisterServiceObject<CServerAdiminstor>();
			std::cout << "Create Service Object!\n";
			return GetService()->Start();
		}
		catch (int _exception)
		{
			std::cout << "Error [" << _exception << "]\n";
			using namespace std;
			std::this_thread::sleep_for(100s);
		}
		return false;
	}
}