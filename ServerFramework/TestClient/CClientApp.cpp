#include "ClientDefines.h"
#include "CClientApp.h"
#include "UCoreInstance.h"
#include "CClientAdiminstor.h"

CClientApp::CClientApp() : 
	m_spClientAdiminstor{nullptr}, 
	m_spCoreInstance{ nullptr }
{
}

void CClientApp::Init()
{
	m_spCoreInstance = Create<UCoreInstance>();
	m_spClientAdiminstor = CreateInitConstructor<CClientAdiminstor>(m_spCoreInstance);
	std::cout << "Start Client!\n";
	if (true == m_spClientAdiminstor->Start())
	{
		return;
	}
}

void CClientApp::Free()
{
	m_spClientAdiminstor.reset();
	m_spCoreInstance.reset();
}
