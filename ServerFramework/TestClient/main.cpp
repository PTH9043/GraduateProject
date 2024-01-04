#include "ClientDefines.h"
#include "CClientApp.h"

int main()
{
	SHPTR<CClientApp> spClientApp = Create<CClientApp>();
	spClientApp->Init();
}