#include "EngineDefine.h"
#include "UProcessedData.h"

UProcessedData::UProcessedData() : m_Data{  },
m_DataType{ 0 }, m_DataSize{ 0 }, m_iNetworkID{ -1 }
{
}

UProcessedData::UProcessedData(const _int _NetworkID, void* _pData, size_t _Size, _int _DataType) :
	m_Data{  }, m_DataType{ _DataType }, m_DataSize{ static_cast<_int>(_Size)  }, m_iNetworkID{_NetworkID}
{
	m_Data = Make::AllocBuffer<_char>(m_DataSize);
	::memcpy(&m_Data[0], _pData, _Size);
}

UProcessedData::UProcessedData(void* _pData, size_t _Size, _int _DataType) :
	m_Data{  }, m_DataType{ _DataType }, m_DataSize{ static_cast<_int>(_Size) }, m_iNetworkID{ 0 }
{
	m_Data = Make::AllocBuffer<_char>(m_DataSize);
	::memcpy(&m_Data[0], _pData, _Size);
}

UProcessedData::UProcessedData(const UProcessedData& _rhs) : m_Data{ _rhs.m_Data },
m_DataType{ _rhs.m_DataType },  m_DataSize{ _rhs.m_DataSize }, m_iNetworkID{_rhs.m_iNetworkID }
{
	_rhs.m_Data = nullptr;
}

UProcessedData::UProcessedData(UProcessedData&& _rhs) noexcept : m_Data{_rhs.m_Data },
m_DataType{_rhs.m_DataType },  m_DataSize{ _rhs.m_DataSize }, m_iNetworkID{ _rhs.m_iNetworkID}
{
	_rhs.m_Data = nullptr;
}

UProcessedData::~UProcessedData()
{
	if (nullptr != m_Data)
	{
		Make::ReleaseBuffer(m_Data);
	}
}
 
UProcessedData& UProcessedData::operator=(const UProcessedData& _other) noexcept
{
	m_Data = _other.m_Data;
	m_DataType = _other.m_DataType;
	m_iNetworkID = _other.m_iNetworkID;
	m_DataSize = _other.m_DataSize;
	m_Data = _other.m_Data;
	_other.m_Data = nullptr;
	return *this;
}

UProcessedData& UProcessedData::operator=(UProcessedData&& _other) noexcept
{
	m_Data = _other.m_Data;
	m_DataType = _other.m_DataType;
	m_iNetworkID = _other.m_iNetworkID;
	m_DataSize = _other.m_DataSize;
	m_Data = _other.m_Data;
	_other.m_Data = nullptr;
	return *this;
}
