#include "EngineDefine.h"
#include "UInputReceiver.h"

UInputReceiver::UInputReceiver(CSHPTRREF<UDevice> _spDevice, _int _KeyContainerNumber) :
	UComponent(_spDevice)
{
	m_KeyContainer.resize(_KeyContainerNumber);
}

UInputReceiver::UInputReceiver(const UInputReceiver& _rhs) : 
	UComponent(_rhs), m_KeyContainer{_rhs.m_KeyContainer}
{
}

_bool UInputReceiver::IsCheckEnableKey(_int _iIndex)
{
	RETURN_CHECK(_iIndex >= m_KeyContainer.size(), false);
	return m_KeyContainer[_iIndex];
}
