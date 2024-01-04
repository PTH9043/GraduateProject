#include "ServerDefines.h"
#include "CServerApp.h"
#include "UCoreInstance.h"
#include "CServerAdiminstor.h"


namespace Server
{
	CServerApp::CServerApp() : m_spCoreInstance{Core::Create<Core::UCoreInstance>() }
	{

	}

	void CServerApp::Init()
	{
		m_spServerAdiminster = Core::CreateInitConstructor<CServerAdiminstor>(m_spCoreInstance);
		std::cout << "Start Server!\n";
		if (true == m_spServerAdiminster->Start())
		{
			return;
		}
	}

	void CServerApp::Free()
	{
		m_spServerAdiminster.reset();
		m_spCoreInstance.reset();
	}
}