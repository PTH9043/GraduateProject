#include "ServerDefines.h"
#include "CServerApp.h"

int main()
{
	Core::SHPTR<Server::CServerApp>	spServerApp = Core::Create<Server::CServerApp>();

	CS_LOGIN csLogin;
	PROTOFUNC::MakeCsLogin("Name", csLogin);
}