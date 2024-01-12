#include "ServerDefines.h"
#include "CServerApp.h"

int main()
{
	Server::CServerApp* Server = CreateAndRegisterApp<Server::CServerApp>();
	Server->Start();
	std::cout << "End Server\n";
}