#include "EngineDefine.h"
#include "UProcessedData.h"

UProcessedData::UProcessedData() : m_pData{ nullptr },
m_DataType{ 0 }, m_DataSize{ 0 }, m_iNetworkID{ -1 }
{
}

UProcessedData::UProcessedData(const _int _NetworkID, void* _pData, size_t _Size, _int _DataType) :
	m_pData{ nullptr }, m_DataType{ _DataType }, m_DataSize{ static_cast<_int>(_Size) }, m_iNetworkID{_NetworkID}
{
	m_pData = Make::AllocBuffer<_char>(_Size);
	::memcpy(m_pData, _pData, _Size);
}

UProcessedData::UProcessedData(const UProcessedData& _rhs) : m_pData{ _rhs.m_pData },
m_DataType{ _rhs.m_DataType.load() },  m_DataSize{ _rhs.m_DataSize.load() }, m_iNetworkID{_rhs.m_iNetworkID.load() }
{
	_rhs.m_pData = nullptr;
}

UProcessedData::UProcessedData(UProcessedData&& _rhs) noexcept : m_pData{_rhs.m_pData},
m_DataType{_rhs.m_DataType.load() },  m_DataSize{ _rhs.m_DataSize.load() }, m_iNetworkID{ _rhs.m_iNetworkID.load() }
{
	_rhs.m_pData = nullptr;
}

UProcessedData::~UProcessedData()
{
	Make::ReleaseBuffer(m_pData);
}
 
UProcessedData& UProcessedData::operator=(const UProcessedData& _other) noexcept
{
	m_pData = _other.m_pData;
	m_DataType = _other.m_DataType.load();
	m_iNetworkID = _other.m_iNetworkID.load();
	m_DataSize = _other.m_DataSize.load();
	_other.m_pData = nullptr;
	return *this;
}

UProcessedData& UProcessedData::operator=(UProcessedData&& _other) noexcept
{
	m_pData = _other.m_pData;
	m_DataType = _other.m_DataType.load();
	m_iNetworkID = _other.m_iNetworkID.load();
	m_DataSize = _other.m_DataSize.load();
	_other.m_pData = nullptr;
	return *this;
}
