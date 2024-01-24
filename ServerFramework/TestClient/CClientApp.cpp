#include "ClientDefines.h"
#include "CClientApp.h"
#include "UCoreInstance.h"
#include "CClientAdiminstor.h"

CClientApp::CClientApp() 
{
}

_bool CClientApp::Start()
{
	std::cout << CurrentMilliseconds() << "\n";
	CreateServiceObject<CClientAdiminstor>();

	std::cout << "Start Client!\n";
	return GetService()->Start();
}
