#include "EngineDefine.h"
#include "CustomMemory.h"

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
		MakeMemoryPool(32, TableIndex, 1024, 32);
		MakeMemoryPool(1024, TableIndex, 2048, 64);
		MakeMemoryPool(2048, TableIndex, 3072, 128);
		MakeMemoryPool(3072, TableIndex, 5120, 256);
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
	UPoolAllocator
	===========================================================
	*/

	class UMemoryAdminster* GAdimister = nullptr;
	class MemoryGlobal {
	public:
		MemoryGlobal()
		{
			GAdimister = new UMemoryAdminster;
		}
		~MemoryGlobal()
		{
			delete GAdimister;
		}
	}MemoryGlobal;

	void* UPoolAllocator::Alloc(size_t _size)
	{
		return GAdimister->Allocate(_size);
	}

	void UPoolAllocator::Release(void* _ptr)
	{
		GAdimister->Release(_ptr);
	}

}