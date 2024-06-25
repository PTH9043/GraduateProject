#pragma once


BEGIN(Engine)

/*
@ Date: 2024-02-03,  Writer: 박태현
@ Explain
- Process를 이용해서 처리된 데이터들을 저장하는 클래스이다.
*/
class UProcessedData {
public:
	UProcessedData();
	UProcessedData(const _int _NetworkID, void * _pData, size_t _Size, _int _DataType);
	UProcessedData(const UProcessedData& _rhs);
	UProcessedData(UProcessedData&& _rhs) noexcept;
	// Send
	template<class T>
		requires CheckProtoType<T>
	UProcessedData(const T& _data, short _tag) : m_DataType{ _tag },
		m_isMake{ true }, m_DataSize{ sizeof(T) }, m_iNetworkID{ 0 }
	{
		m_pData = Make::AllocBuffer<_char>(m_DataSize);
		_data.SerializePartialToArray((void*)&m_pData[0], static_cast<int>(_data.ByteSizeLong()));
	}
	// Recv
	template<class T>
		requires CheckProtoType<T>
	UProcessedData(const _int _NetworkID, const T& _data, short _tag) : m_DataType{ _tag }, 
		m_isMake{ true }, m_DataSize{ sizeof(T) }, m_iNetworkID{ _NetworkID }
	{
		m_pData = Make::AllocBuffer<_char>(m_DataSize);
		_data.SerializePartialToArray((void*)&m_pData[0], static_cast<int>(_data.ByteSizeLong()));
	}
	// Recv
	template<class T>
		requires CheckProtoType<T>
	UProcessedData(const _int _NetworkID, T* _data, short _tag) : m_pData{ reinterpret_cast<_char*>(_data)}, m_DataType { _tag },
		m_isMake{ true }, m_DataSize{ sizeof(T) }, m_iNetworkID{ _NetworkID } { }

	~UProcessedData();

	UProcessedData& operator=(const UProcessedData& _other) = delete;
	UProcessedData& operator=(UProcessedData&& _other) noexcept;
	template<class T>
	T* ConvertData() { return reinterpret_cast<T*>(m_pData); }
public: /* get Set */
	_int GetDataID() const { return m_iNetworkID; }
	_int GetDataType() const { return m_DataType; }
	_int GetDataSize() const { return m_DataSize; }
	_char* GetData() const { return m_pData; }
private:
	_int								m_iNetworkID;
	_char*							m_pData;                     
	_int								m_DataType;
	_int								m_DataSize;
	// 이렇게 안하면 데이터가 삭제되는 현상이 생김, 그렇다고 해당 클래스를 할당하기는 싫음
	mutable _bool			m_isMake;
};

END

namespace std {
/*
@ Date: 2024-02-03,  Writer: 박태현
@ Explain
- HASH opeartor() 연산에 대한 UProcessedData 재정의, concurrent_ 컨테이너에 사용하기 위함
*/
	template <>
	struct std::hash<Engine::UProcessedData> {
		size_t operator()(Engine::UProcessedData&& ptr) const {
			return std::hash<char*>()(ptr.GetData());
		}
	};
	/*
	@ Date: 2024-02-03,  Writer: 박태현
	@ Explain
	- Equal 연산에 대한 UProcessedData 재정의, concurrent_ 컨테이너에 사용하기 위함
	*/
	template <>
	struct std::equal_to<Engine::UProcessedData> {
		bool operator()(Engine::UProcessedData&& lhs, Engine::UProcessedData&& rhs) const {
			return lhs.GetData() == rhs.GetData();
		}
	};
	/*
	@ Date: 2024-02-03,  Writer: 박태현
	@ Explain
	- less 연산에 대한 UProcessedData 재정의, concurrent_ 컨테이너에 사용하기 위함
	*/
	template <>
	struct std::less<Engine::UProcessedData> {
		bool operator()(Engine::UProcessedData&& lhs, Engine::UProcessedData&& rhs) const {
			return lhs.GetData() < rhs.GetData();
		}
	};
}