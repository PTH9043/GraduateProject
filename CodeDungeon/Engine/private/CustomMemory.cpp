#include "EngineDefine.h"
#include "CustomMemory.h"
#include "UGameInstance.h"

namespace Engine {

	UMemoryPool::UMemoryPool(_int _AllocSize) :
		m_MemoryQueue{}, m_AllocSize{ _AllocSize }
	{
	}

	UMemoryPool::~UMemoryPool()
	{
		while (m_MemoryQueue.empty() == false)
		{
			UMemoryHeader* header;
			m_MemoryQueue.try_pop(header);
			::free(header);
		}
	}

	void UMemoryPool::Push(UMemoryHeader* ptr)
	{
		ptr->SetAllocSize(0);

		m_MemoryQueue.push(ptr);
	}

	UMemoryHeader* UMemoryPool::Pop()
	{
		UMemoryHeader* Ptr{ nullptr };
		if (false == m_MemoryQueue.empty())
		{
			m_MemoryQueue.try_pop(Ptr);
		}
		else
		{
			Ptr = reinterpret_cast<UMemoryHeader*>(::malloc(m_AllocSize));
		}
		return Ptr;
	}

	/*
	===========================================================
	UMemoryPool
	===========================================================
	UMemoryAdminster
	===========================================================
	*/

	UMemoryAdminster::UMemoryAdminster()
	{
		_uint Size = 16;
		_uint TableIndex = 0;
	//	MakeMemoryPool(16, TableIndex, 512, 16);
		MakeMemoryPool(32, REF_OUT TableIndex, 1024, 32);
		MakeMemoryPool(1024, REF_OUT TableIndex, 2048, 64);
		MakeMemoryPool(2048, REF_OUT TableIndex, 3072, 128);
		MakeMemoryPool(3072, REF_OUT  TableIndex, 5120, 256);
	}

	UMemoryAdminster::~UMemoryAdminster()
	{
		for (auto& iter : m_Pools)
		{
			delete iter;
		}
		m_Pools.clear();
	}

	void* UMemoryAdminster::Allocate(_ullong _Size)
	{
		UMemoryHeader* Header = nullptr;
		const _ullong AllocateSize = _Size + sizeof(UMemoryHeader);
		// 메모리 최대 크기를 꺼내오면 일반 할당 
		if (AllocateSize > MAX_ALLOC_SIZE)
		{
			Header = reinterpret_cast<UMemoryHeader*>(::malloc(AllocateSize));
		}
		else
		{
			// 메모리 풀에서 꺼내온다. 
			Header = m_PoolTable[AllocateSize]->Pop();
		}
		return UMemoryHeader::AttachHeader(Header, AllocateSize);
	}

	void UMemoryAdminster::Release(void* _Ptr)
	{
		UMemoryHeader* Header = UMemoryHeader::DetachHeader(_Ptr);

		const _ullong AllocSize = Header->GetAllocSize();
		if (AllocSize > MAX_ALLOC_SIZE)
		{
			::free(Header);
		}
		else
		{
			m_PoolTable[AllocSize]->Push(Header);
		}
	}

	void UMemoryAdminster::MakeMemoryPool(_uint _Size, _uint& _TableIndex, const _uint _Limited, const _uint _AddValue)
	{
		for (; _Size <= _Limited;)
		{
			UMemoryPool* pool = new UMemoryPool(_Size);
			m_Pools.push_back(pool);

			while (_TableIndex <= _Size)
			{
				m_PoolTable[_TableIndex] = pool;
				++_TableIndex;
			}
			_Size += _AddValue;
		}
	}
	/*
	===========================================================
	UMemoryAdminster
	===========================================================
	UBaseAllocator
	===========================================================
	*/
	void* UBaseAllocator::Alloc(size_t size)
	{
		return ::malloc(size);
	}

	void UBaseAllocator::Release(void* ptr)
	{
		::free(ptr);
	}
	/*
	===========================================================
	UBaseAllocator
	===========================================================
	UPoolAllocator
	===========================================================
	*/

	/*
	
	*/
	class MemoryGlobal {
	public:
		MemoryGlobal() : m_pMemoryAdiminster{ nullptr } {
			m_pMemoryAdiminster = new UMemoryAdminster;
		}
		~MemoryGlobal() {
			delete m_pMemoryAdiminster;
		}
	public:
		UMemoryAdminster* GetMemoryAdiminster() const { return m_pMemoryAdiminster; }

	private:
		UMemoryAdminster* m_pMemoryAdiminster;
	};
	static MemoryGlobal MGlobal;

	void* UPoolAllocator::Alloc(size_t _size)
	{
		return MGlobal.GetMemoryAdiminster()->Allocate(_size);
	}

	void UPoolAllocator::Release(void* _ptr)
	{
		MGlobal.GetMemoryAdiminster()->Release(_ptr);
	}

}