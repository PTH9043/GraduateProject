#include "CoreDefines.h"
#include "URandomManager.h"

namespace Core
{
	URandomManager::URandomManager() :
		m_UniformDistribution{  }
	{
	}

	_int URandomManager::ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum)
	{
		_int number = _iMinNum + (m_UniformDistribution(m_dre)) % _iMaxNum;
		return number;
	}

	_int URandomManager::ReturnRadomNumber(const _int _iMaxNum)
	{
		_int number =  (m_UniformDistribution(m_dre)) % _iMaxNum;
		return number;
	}

	void URandomManager::Free()
	{
	}

}