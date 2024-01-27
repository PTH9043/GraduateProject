#include "ServerDefines.h"
#include "CServerApp.h"

#include "UDBConnector.h"

int main()
{
	// TODO: 여기에 코드를 입력합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Server::CServerApp* Server = CreateAndRegisterApp<Server::CServerApp>();
	Server->Start();
	std::cout << "End Server\n";
}