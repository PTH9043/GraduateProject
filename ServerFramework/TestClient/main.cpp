#include "ClientDefines.h"
#include "CClientApp.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SHPTR<CClientApp> spClientApp = Create<CClientApp>();
	spClientApp->Init();
}