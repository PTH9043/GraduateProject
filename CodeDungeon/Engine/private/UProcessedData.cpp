#include "EngineDefine.h"
#include "UProcessedData.h"

UProcessedData::UProcessedData(void* _pData, size_t _Size, _int _DataType) : 
	m_pData{nullptr}, m_DataType{_DataType}, m_isMake{true}
{
	m_pData = Make::AllocBuffer(_Size);
	::memcpy(m_pData, _pData, _Size);
}

UProcessedData::UProcessedData(const UProcessedData& _rhs) : m_pData{ _rhs.m_pData },
m_DataType{ _rhs.m_DataType }, m_isMake{ _rhs.m_isMake }
{
	_rhs.m_isMake = false;
}

UProcessedData::UProcessedData(UProcessedData&& _rhs) noexcept : m_pData{_rhs.m_pData},
m_DataType{_rhs.m_DataType }, m_isMake{ true }
{
	_rhs.m_pData = nullptr;
}

UProcessedData::~UProcessedData()
{
	RETURN_CHECK(false == m_isMake, ;);
	Make::ReleaseBuffer(m_pData);
}
 
UProcessedData& UProcessedData::operator=(UProcessedData&& _other) noexcept
{
	m_pData = _other.m_pData;
	m_DataType = _other.m_DataType;
	_other.m_pData = nullptr;
	m_isMake = true;
	return *this;
}
