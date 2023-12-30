#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H

namespace Core {

	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	*/
	/*
	@ Date: 2023-12-30
	@ Writer: 박태현
	@ Explain
	- 기본적인 메모리 할당과 Delete를 맡고 있는 클래스이다. 
	*/
	class BaseAllocator {
	public:
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		*/
		static void* Alloc(_ullong _size);
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		*/
		static void Release(void* _ptr);
	};


	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
	/*
	@ Date: 2023-12-30
	@ Writer: 박태현
	@ Explain
	- 해당하는 OS에 맞게 가상 메모리를 할당해주는 클래스이다.
	*/
	class StompAllocator{
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		@ Explain
		- 메모리를 할당할 때 PAGE_SIZE 만큼 잡아주는 것이다. 
		그래서 메모리를 할당할 때 Page_Size만큼 할당하기 위해서 사용
		*/
		enum { PAGE_SIZE = 0x1000 };

	public:
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		*/
		static void* Alloc(_ullong _size);

#ifdef WINDOW_OS
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		*/
		static void		Release(void* _ptr);
#else
		static void		Release(void* _ptr, _ullong _size);
#endif
	};

	/*
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H