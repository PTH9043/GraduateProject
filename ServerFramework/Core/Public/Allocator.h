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
	class CORE_DLL UBaseAllocator {
	public:
		/*
		@ Date: 2023-12-30
		@ Writer: 박태현
		*/
		static void* Alloc(size_t _size);
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
	POOL ALLOCATOR
	----------------------------------------
	*/
		/*
		@ Date: 2023-12-31, Writer: 박태현
		@ Explain
		- 메모리 풀 객체 할당을 위한 클래스
		*/
		class CORE_DLL UPoolAllocator {
		public:
			/*
			@ Date: 2023-12-30
			@ Writer: 박태현
			*/
			static void* Alloc(size_t _size);
			/*
			@ Date: 2023-12-30
			@ Writer: 박태현
			*/
			static void Release(void* _ptr);
	};



	/*
	----------------------------------------
	POOL ALLOCATOR
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
	class CORE_DLL UStompAllocator{
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
		static void* Alloc(size_t _size);

#ifdef _WIN32
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
	STL ALLOCATOR
	----------------------------------------
	*/

	template<class T>
	class STLAllocator {
	public:
		
		STLAllocator() {}

		template<class Other>
		STLAllocator(const STLAllocator<Other>&) {}

		T* Allocate(size_t _size) {
			const size_t size = _size * sizeof(T);
			return static_cast<T*>(UPoolAllocator::Alloc(size));
		}

		void Deallocate(T* _Ptr, size_t _size) {
			UPoolAllocator::Release(_Ptr);
		}

	private:
		// 다른 형식의 Allocator가 같은 종류로 간주되게 함
		template <class U>
		friend class STLAllocator;
	};

	template <class T1, class T2>
	bool operator==(const STLAllocator<T1>&, const STLAllocator<T2>&) noexcept {
		return true;
	}

	template <class T1, class T2>
	bool operator!=(const STLAllocator<T1>&, const STLAllocator<T2>&) noexcept {
		return false;
	}


	/*
	----------------------------------------
	STL ALLOCATOR
	----------------------------------------
	MEMORY ALLOC FUNC
	----------------------------------------
	*/

	template<class Type, class... Args>
	Type* MemoryAlloc(Args&&... args)
	{
		Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
		new(memory)Type(std::forward<Args>(args)...); // placement new
		return memory;
	}

	template<class Type>
	void MemoryRelease(Type* obj)
	{
		obj->~Type();
		UPoolAllocator::Release(obj);
	}
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H