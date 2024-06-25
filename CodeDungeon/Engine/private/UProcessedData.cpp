#include "EngineDefine.h"
#include "UProcessedData.h"

UProcessedData::UProcessedData() : m_pData{ nullptr },
m_DataType{ 0 }, m_DataSize{ 0 },
m_isMake{ false }, m_iNetworkID{0}
{
}

UProcessedData::UProcessedData(const _int _NetworkID, void* _pData, size_t _Size, _int _DataType) :
	m_pData{ nullptr }, m_DataType{ _DataType }, m_isMake{ true }, m_DataSize{ static_cast<_int>(_Size) }, m_iNetworkID{_NetworkID}
{
	m_pData = Make::AllocBuffer<_char>(_Size);
	::memcpy(m_pData, _pData, _Size);
}

UProcessedData::UProcessedData(const UProcessedData& _rhs) : m_pData{ _rhs.m_pData },
m_DataType{ _rhs.m_DataType }, m_isMake{ _rhs.m_isMake }, m_DataSize{ _rhs.m_DataSize }, m_iNetworkID{_rhs.m_iNetworkID}
{
	_rhs.m_isMake = false;
}

UProcessedData::UProcessedData(UProcessedData&& _rhs) noexcept : m_pData{_rhs.m_pData},
m_DataType{_rhs.m_DataType }, m_isMake{ true }, m_DataSize{ _rhs.m_DataSize }, m_iNetworkID{ _rhs.m_iNetworkID }
{
	_rhs.m_pData = nullptr;
	_rhs.m_isMake = false;
}

UProcessedData::~UProcessedData()
{
	RETURN_CHECK(false == m_isMake, ;);
	if (nullptr == m_pData)
	{
		Make::ReleaseBuffer(m_pData);
	}
}
 
UProcessedData& UProcessedData::operator=(UProcessedData&& _other) noexcept
{
	m_pData = _other.m_pData;
	m_DataType = _other.m_DataType;
	m_iNetworkID = _other.m_iNetworkID;
	m_DataType = _other.m_DataType;
	m_DataSize = _other.m_DataSize;
	_other.m_pData = nullptr;
	m_isMake = true;
	return *this;
}
