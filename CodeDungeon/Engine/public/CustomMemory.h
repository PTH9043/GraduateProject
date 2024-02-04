#pragma once

namespace Engine {
	/*
	@ Date: 2024-02-04, Writer: 박태현
	@ Explain
	-  기본 메모리 사이즈 (Memory Pool 최적화를 위함)
	*/
	enum
	{
		BASE_ALLOC_SIZE = 16
	};

	/*
	===========================================================
	MemoryHeader
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: 박태현
	@ Explain
	-  메모리 앞에 붙는 헤더
	*/
	class UMemoryHeader {
	public:
		UMemoryHeader(_llong _Size) : m_AllocSize{ _Size } {}

		static void* AttachHeader(UMemoryHeader* _Header, _llong _Size) {
			new(_Header)UMemoryHeader(_Size);
			return reinterpret_cast<void*>(++_Header);
		}

		static UMemoryHeader* DetachHeader(void* _Ptr) {
			UMemoryHeader* header = reinterpret_cast<UMemoryHeader*>(_Ptr) - 1;
			return header;
		}

		_llong GetAllocSize() const { return m_AllocSize; }
		void SetAllocSize(const _llong _AllocSize) { this->m_AllocSize = _AllocSize; }
	private:
		_llong	m_AllocSize;
	};
	/*
	===========================================================
	MemoryHeader
	===========================================================
	MemoryPool
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: 박태현
	@ Explain
	-  메모리리를 저장할 Memory Pool이다.
	*/
	class UMemoryPool {
	public:
		UMemoryPool(_int _AllocSize);
		~UMemoryPool();

		void			Push(UMemoryHeader* ptr);
		UMemoryHeader* Pop();

	private:
		CONPRIORITYQUEUE<UMemoryHeader*>		m_MemoryQueue;
		_int																			m_AllocSize;
	};
	/*
	===========================================================
	MemoryPool
	===========================================================
	MemoryAdminster
	===========================================================
	*/
	/*
	@ Date: 2024-02-04, Writer: 박태현
	@ Explain
	-  메모리 풀을 관리하는 클래스
	*/
	class UMemoryAdminstor {
		enum
		{
			POOL_COUNT = 350,
			MAX_ALLOC_SIZE = BASE_ALLOC_SIZE * POOL_COUNT
		};
	public:
		UMemoryAdminstor();
		~UMemoryAdminstor();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, const _uint _Limited, const _uint _AddValue);

	private:
		// 메모리를 빠르게 찾기 위한 풀 테이블이다. 
		ARRAY<UMemoryPool*, POOL_COUNT>		m_PoolTable;
		CONUNOMAP<_ullong, int>							m_KeyTable;
	};
	/*
	===========================================================
	MemoryAdminster
	===========================================================
	BaseAllocator
	===========================================================
	*/
	/*
	@ Date: 2024-02-01,  Writer: 박태현
	@ Explain
	- 기본적인 할당을 위한 클래스
	*/
	class UBaseAllocator
	{
	public:
		static void* Alloc(size_t size);
		static void	Release(void* ptr);
	};
		/*
	===========================================================
	BaseAllocator
	===========================================================
	PoolAllocator
	===========================================================
	*/
	/*
	@ Date: 2024-02-01,  Writer: 박태현
	@ Explain
	- Memory Adiminster으로 메모리 관리
	*/
	class UPoolAllocator {
	public:
		static void* Alloc(size_t _size);
		static void Release(void* _ptr);
		static void Release(const void* _ptr);
	};
	/*
	===========================================================
	UPoolAllocator
	===========================================================
	UObjectPool
	===========================================================
	*/
	/*
	@ Date: 2024-02-02,  Writer: 박태현
	@ Explain
	- ObjectPool 클래스당 Memory Pool
	*/
	template<class Type>
	class UObjectPool {
	public:
		template<class... Args>
		static Type* Pop(Args&&... _args) {
			Type* memory = static_cast<Type*>(UMemoryHeader::AttachHeader(s_MemoryPool.Pop(), s_AllocSize));
			new(memory)Type(std::forward<Args>(_args)...); // placement new
			return memory;
		}
		static void Push(Type* obj)
		{
			obj->~Type();
			s_MemoryPool.Push(UMemoryHeader::DetachHeader(obj));
		}

		template<class... Args>
		static std::shared_ptr<Type> MakeShared(Args&&... _args) {
			std::shared_ptr<Type> ptr = { Pop(std::forward<Args>(_args)...), Push };
			return std::move(ptr);
		}
	private:
		static UMemoryPool	s_MemoryPool;
		static _int						s_AllocSize;
	};

	template<typename Type>
	_int UObjectPool<Type>::s_AllocSize = sizeof(Type) + sizeof(UMemoryHeader);

	template<typename Type>
	UMemoryPool UObjectPool<Type>::s_MemoryPool{ s_AllocSize };

	/*
	===========================================================
	UObjectPool
	===========================================================
	Make
	===========================================================
	*/

	namespace Make {

		/*
		@ Date: 2024-02-03,  Writer: 박태현
		@ Explain
		- 할당을 내가 custom 하기 위한 함수
		*/
		template<typename Type, typename... Args>
		static Type* xnew(Args&&... args)
		{
			Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
			new(memory)Type(std::forward<Args>(args)...); // placement new
			return memory;
		}
		/*
		@ Date: 2024-02-03,  Writer: 박태현
		@ Explain
		- 마음데로 할당한 값을 삭제하는 함수
		*/
		template<typename Type>
		static void xdelete(Type* obj)
		{
			obj->~Type();
			UPoolAllocator::Release(obj);
		}
		/*
		@ Date: 2024-02-04,  Writer: 박태현
		@ Explain
		- 배열을 할당하기 위한 함수
		*/
		template<class T>
		static T* AllocBuffer(size_t _number) {
			T* p = static_cast<T*>(UPoolAllocator::Alloc(_number * sizeof(T)));
			return p;
		}
		/*
		@ Date: 2024-02-03,  Writer: 박태현
		@ Explain
		- 배열을 제거하기 위한 함수
		*/
		template<class T>
		static void ReleaseBuffer(T* obj)
		{
			UPoolAllocator::Release(obj);
		}
		/*
		@ Date: 2024-02-03,  Writer: 박태현
		@ Explain
		- const 용 배열을 제거하기 위한 함수
		*/
		template<class T>
		static void ReleaseBuffer(const T* obj)
		{
			UPoolAllocator::Release(obj);
		}
		/*
		@ Date: 2024-02-03,  Writer: 박태현
		@ Explain
		- make_shared를 custom
		*/
		template<typename Type, typename... Args>
		static std::shared_ptr<Type> MakeShared(Args&&... args)
		{
			return std::shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
		}
	}
}