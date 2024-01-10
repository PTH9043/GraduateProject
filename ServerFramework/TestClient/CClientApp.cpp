#include "ClientDefines.h"
#include "CClientApp.h"
#include "UCoreInstance.h"
#include "CClientAdiminstor.h"

CClientApp::CClientApp() 
{
}

_bool CClientApp::Start()
{
	CreateServiceObject<CClientAdiminstor>();
	std::cout << "Start Client!\n";
	return GetService()->Start();
}
