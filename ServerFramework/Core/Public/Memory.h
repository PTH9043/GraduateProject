#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H

#include "Allocator.h"

namespace Core {

	/*
	@ Date: 2023-12-31
	@ Writer: 박태현
	@ Explain
	- 메모리의 앞 부분을 나타내는 Header이다.
	*/
	struct  MEMORYHEADER {

		MEMORYHEADER(_llong _Size) : AllocateSize{ _Size } {}

		static void* AttachHeader(MEMORYHEADER* _Header, _llong _Size)
		{
			new(_Header)MEMORYHEADER(_Size);
			// 메모리 헤더 부분을 건너 뛴다. 
			return reinterpret_cast<void*>(++_Header);
		}

		static MEMORYHEADER* DetachHeader(void* _Ptr)
		{
			// 메모리 헤더 부분을 추출한다. 
			MEMORYHEADER* Header = reinterpret_cast<MEMORYHEADER*>(_Ptr) - 1;
			return Header;
		}

		volatile _llong		AllocateSize;
	};

	/*
	-------------------------------
	MEMORYHEADER
	------------------------------
	MEMORYPOOL
	------------------------------
	*/

	/*
	@ Date: 2023-12-31, Writer: 박태현
	@ Explain
	- 메모리들을 저장하는 클래스
	*/
	class CORE_DLL AMemoryPool {
		using MEMORYQUEUE = CONQUEUE<MEMORYHEADER*>;
	public:
		AMemoryPool(const _ullong& _AllocateSize);
		~AMemoryPool();

		void Push(MEMORYHEADER* _ptr);
		MEMORYHEADER* Pop();
	private:
		_ullong														m_AllocateSize;
		ATOMIC<_uint>										m_AllocateCount;
		// Memory Queue
		MEMORYQUEUE										m_MemoryQueue;
	};


	/*
	-------------------------------
	MEMORYPOOL
	------------------------------
	MEMORYADMINSTER
	------------------------------
	*/
	/*
	@ Date: 2023-12-31
	@ Writer: 박태현
	@ Explain
	- 메모리를 관리하는 관리자 클래스
	*/
	class CORE_DLL AMemoryAdminster {
		enum
		{
			POOL_COUNT = (512 / 16) + (512 / 32) + (1024 / 64) + (1024 / 128) + (2048 / 256),
			MAX_ALLOC_SIZE = 5120
		};
	public: 
		AMemoryAdminster();
		~AMemoryAdminster();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, REF_IN _uint& _TableIndex,
			const _uint _Limited, const _uint _AddValue);

	private:
		// 메모리를 빠르게 찾기 위한 풀 테이블이다. 
		ARRAY<AMemoryPool*, MAX_ALLOC_SIZE + 1>		m_PoolTable;

		CONVECTOR<AMemoryPool*>										m_Pools;
	};

}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H