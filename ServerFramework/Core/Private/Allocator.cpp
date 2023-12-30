#include "CoreDefines.h"
#include "Allocator.h"

namespace Core
{
	void* BaseAllocator::Alloc(_ullong  _size)
	{
		return ::malloc(_size);
	}

	void BaseAllocator::Release(void* _ptr)
	{
		::free(_ptr);
	}


	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	STOMP
	----------------------------------------
	*/


	void* StompAllocator::Alloc(_ullong _size)
	{
		// 반올림을 위한 코드
		const _ullong PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		// PageCount에 뒤에 부분에 할당할 수 있게 DataOffset 조정해서 오버플로우 문제 해결
		const _ullong DataOffset = PageCount * PAGE_SIZE - _size;
#ifdef WINDOW_OS
		void* BaseAddress = ::VirtualAlloc(NULL, PageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
		void* BaseAddress = mmap(NULL, PageCount * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
		return static_cast<void*>(static_cast<_byte*>(BaseAddress) + DataOffset);
	}
#ifdef WINDOW_OS
	void StompAllocator::Release(void* _ptr)
	{
		// DataOffset을 조정해서 할당한 메모리를 원래 자리로 돌아가게 만든다. 
		const _llong address = reinterpret_cast<_llong>(_ptr);
		const _llong baseAddress = address - (address % PAGE_SIZE);
		::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
	}
#else
	void StompAllocator::Release(void* _ptr, _ullong _size)
	{	
		const _ullong PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		const _llong address = reinterpret_cast<_llong>(_ptr);
		const _llong BaseAddress = address - (address % PAGE_SIZE);
		munmap(reinterpret_cast<_byte*>(BaseAddress), PageCount * PAGE_SIZE);
	}
#endif

	/*
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
}