#include "EngineDefine.h"
#include "CustomMemory.h"
#include "UGameInstance.h"

namespace Engine {

	UMemoryPool::UMemoryPool(_int _AllocSize) :
		m_MemoryQueue{}, m_AllocSize{ MakeMultipleNumber<BASE_ALLOC_SIZE>(_AllocSize)}
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

	UMemoryAdminstor::UMemoryAdminstor()
	{
		_uint Size = BASE_ALLOC_SIZE;
		MakeMemoryPool(Size, POOL_COUNT, Size);
	}

	UMemoryAdminstor::~UMemoryAdminstor()
	{
		for (auto& iter : m_PoolTable)
		{
			delete iter;
		}
	}

	void* UMemoryAdminstor::Allocate(_ullong _Size)
	{
		UMemoryHeader* Header = nullptr;
		_ullong AllocateSize = _Size + sizeof(UMemoryHeader);
		AllocateSize = MakeMultipleNumber<BASE_ALLOC_SIZE>(AllocateSize);
		// 메모리 최대 크기를 꺼내오면 일반 할당 
		if (AllocateSize > MAX_ALLOC_SIZE)
		{
			Header = reinterpret_cast<UMemoryHeader*>(::malloc(AllocateSize));
		}
		else
		{
			// 메모리 풀에서 꺼내온다. 
			Header = m_PoolTable[m_KeyTable[AllocateSize]]->Pop();
		}
		return UMemoryHeader::AttachHeader(Header, AllocateSize);
	}

	void UMemoryAdminstor::Release(void* _Ptr)
	{
		UMemoryHeader* Header = UMemoryHeader::DetachHeader(_Ptr);

		const _ullong AllocSize = Header->GetAllocSize();
		if (AllocSize > MAX_ALLOC_SIZE)
		{
			::free(Header);
		}
		else
		{
			m_PoolTable[m_KeyTable[AllocSize]]->Push(Header);
		}
	}

	void UMemoryAdminstor::MakeMemoryPool(_uint _Size, const _uint _Limited, const _uint _AddValue)
	{
		_uint Index{ 0 };
		for (; Index < _Limited;)
		{
			UMemoryPool* pool = new UMemoryPool(_Size);
			m_PoolTable[Index] = pool;
			m_KeyTable.insert(MakePair(_Size, Index));
			++Index;
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
			m_pMemoryAdiminster = new UMemoryAdminstor;
		}
		~MemoryGlobal() {
			delete m_pMemoryAdiminster;
		}
	public:
		UMemoryAdminstor* GetMemoryAdiminster() const { return m_pMemoryAdiminster; }

	private:
		UMemoryAdminstor* m_pMemoryAdiminster;
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

	void UPoolAllocator::Release(const void* _ptr)
	{
		MGlobal.GetMemoryAdiminster()->Release(const_cast<void*>(_ptr));
	}
}