#include "CoreDefines.h"
#include "Memory.h"

namespace Core
{
	AMemoryPool::AMemoryPool(const _ullong& _AllocateSize) : 
		m_AllocateSize{ MakeMultipleNumber<BASE_ALLOC_SIZE>(_AllocateSize) }, m_AllocateCount{0}
	{

	}

	AMemoryPool::~AMemoryPool()
	{
		while (!m_MemoryQueue.empty())            
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
	AMemoryAdiminstor::AMemoryAdiminstor()
	{
		_uint Size = BASE_ALLOC_SIZE;
		MakeMemoryPool(Size, POOL_COUNT, Size);
	}

	AMemoryAdiminstor::~AMemoryAdiminstor()
	{
		for (auto& iter : m_PoolTable)
		{
			delete iter;
		}
	}

	// SIze의 크기가 해당 메모리에 없으면 그냥 할당, 아니라면 해제한다. 
	void* AMemoryAdiminstor::Allocate(_ullong _Size)
	{
		MEMORYHEADER* Header = nullptr;
		_ullong AllocateSize = _Size + sizeof(MEMORYHEADER);
#ifdef USE_STOMP
		Header = reinterpret_cast<MEMORYHEADER*>(UStompAllocator::Alloc(_Size));
#else
		AllocateSize = MakeMultipleNumber<BASE_ALLOC_SIZE>(AllocateSize);
		// 메모리 최대 크기를 꺼내오면 일반 할당 
		if (AllocateSize > MAX_ALLOC_SIZE)
		{
			Header = reinterpret_cast<MEMORYHEADER*>(::malloc(_Size));
		}
		else
		{
			// 메모리 풀에서 꺼내온다. 
			Header = m_PoolTable[m_KeyTable[AllocateSize]]->Pop();
		}
#endif
		return MEMORYHEADER::AttachHeader(Header, AllocateSize);
	}
	// SIze의 크기가 해당 메모리에 없으면 해제, 아니면 반납한다. 
	void AMemoryAdiminstor::Release(void* _Ptr)
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
			m_PoolTable[m_KeyTable[AllocSize]]->Push(Header);
		}
#endif
	}
	// 메모리 할당을 해주는 함수이다. 
	void AMemoryAdiminstor::MakeMemoryPool(_uint _Size, const _uint _Limited, const _uint _AddValue)
	{
		_uint index = 0;
		for (; index < _Limited;)
		{
			AMemoryPool* pool = new AMemoryPool(_Size);
			m_PoolTable[index] = pool;
			m_KeyTable.insert(MakePair(_Size, index));
			++index;
			_Size += _AddValue;
		}
	}

}