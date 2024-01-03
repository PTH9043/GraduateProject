#include "ServerDefines.h"
#include "CServerApp.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Core::SHPTR<Server::CServerApp>	spServerApp = Core::Create<Server::CServerApp>();
}