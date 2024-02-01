#include "CoreDefines.h"
#include "Memory.h"

namespace Core
{
	AMemoryPool::AMemoryPool(const _ullong& _AllocateSize) : 
		m_AllocateSize{_AllocateSize}, m_AllocateCount{0}
	{

	}

	AMemoryPool::~AMemoryPool()
	{
		while (m_MemoryQueue.empty() == false)
		{
			MEMORYHEADER* header;
			m_MemoryQueue.try_pop(header);
			::free(header);
		}
	}
	/*
	* 반납된 메모리를 풀에 저장한다. 
	*/
	void AMemoryPool::Push(MEMORYHEADER* _ptr)
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		MEMORYHEADER* Ptr = _ptr;
		{
			Ptr->AllocateSize = 0;
		}
		m_MemoryQueue.push(std::move(_ptr));
		m_AllocateCount.fetch_add(1);
	}

	MEMORYHEADER* AMemoryPool::Pop()
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		MEMORYHEADER* Header = nullptr;
		if (false == m_MemoryQueue.empty())
		{
			m_MemoryQueue.try_pop(Header);
		}

		if (nullptr == Header)
		{
			Header = reinterpret_cast<MEMORYHEADER*>(::malloc(m_AllocateSize));
		}
		else
		{
			ASSERT_CRASH(Header->AllocateSize == 0);
			m_AllocateCount.fetch_sub(1);
		}

		return Header;
	}

	/*
	-------------------------------
	MEMORYPOOL
	------------------------------
	MEMORYADMINSTER
	------------------------------
	*/

	// 저장할 메모리 할당
	AMemoryAdminster::AMemoryAdminster()
	{
		_uint Size = 16;
		_uint TableIndex = 0;
		MakeMemoryPool(16, REF_OUT TableIndex, 512, 16);
		MakeMemoryPool(512, REF_OUT TableIndex, 1024, 32);
		MakeMemoryPool(1024, REF_OUT TableIndex, 2048, 64);
		MakeMemoryPool(2048 , REF_OUT TableIndex, 3072, 128);
		MakeMemoryPool(3072, REF_OUT TableIndex, 5120, 256);
	}

	AMemoryAdminster::~AMemoryAdminster()
	{
		for (auto& iter : m_Pools)
		{
			delete iter;
		}
		m_Pools.clear();
	}

	// SIze의 크기가 해당 메모리에 없으면 그냥 할당, 아니라면 해제한다. 
	void* AMemoryAdminster::Allocate(_ullong _Size)
	{
		MEMORYHEADER* Header = nullptr;
		const _ullong AllocateSize = _Size + sizeof(MEMORYHEADER);
#ifdef USE_STOMP
		Header = reinterpret_cast<MEMORYHEADER*>(UStompAllocator::Alloc(_Size));
#else
		// 메모리 최대 크기를 꺼내오면 일반 할당 
		if (AllocateSize > MAX_ALLOC_SIZE)
		{
			Header = reinterpret_cast<MEMORYHEADER*>(::malloc(_Size));
		}
		else
		{
			// 메모리 풀에서 꺼내온다. 
			Header = m_PoolTable[AllocateSize]->Pop();
		}
#endif
		return MEMORYHEADER::AttachHeader(Header, AllocateSize);
	}
	// SIze의 크기가 해당 메모리에 없으면 해제, 아니면 반납한다. 
	void AMemoryAdminster::Release(void* _Ptr)
	{
		MEMORYHEADER* Header = MEMORYHEADER::DetachHeader(_Ptr);

		const _ullong AllocSize = Header->AllocateSize;
		ASSERT_CRASH(AllocSize > 0);

#ifdef USE_STOMP
		UStompAllocator::Release(_Ptr);
#else
		if (AllocSize > MAX_ALLOC_SIZE)
		{
			::free(Header);
		}
		else
		{
			m_PoolTable[AllocSize]->Push(Header);
		}
#endif
	}
	// 메모리 할당을 해주는 함수이다. 
	void AMemoryAdminster::MakeMemoryPool(_uint _Size, REF_IN _uint& _TableIndex,
		const _uint _Limited, const _uint _AddValue)
	{
		for (; _Size <= _Limited;)
		{
			AMemoryPool* pool = new AMemoryPool(_Size);
			m_Pools.push_back(pool);

			while (_TableIndex <= _Size)
			{
				m_PoolTable[_TableIndex] = pool;
				++_TableIndex;
			}
			_Size += _AddValue;
		}
	}

}