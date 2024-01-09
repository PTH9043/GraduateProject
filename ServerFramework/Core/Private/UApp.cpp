#include "CoreDefines.h"
#include "UApp.h"

namespace Core
{
	UApp::UApp() :
		m_spCoreInstance{ Create<UCoreInstance>() },
		 m_spService{nullptr}
	{

	}

	UApp::~UApp()
	{
	}

	_bool UApp::Start()
	{

		return true;
	}
}