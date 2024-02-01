#pragma once

namespace Engine {
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
		CONQUEUE<UMemoryHeader*>		m_MemoryQueue;
		_int															m_AllocSize;
	};
	/*
	===========================================================
	MemoryPool
	===========================================================
	MemoryAdminster
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: 박태현
	@ Explain
	-  메모리 풀을 관리하는 클래스
	*/
	class UMemoryAdminster {
		enum
		{
			POOL_COUNT = (512 / 16) + (512 / 32) + (1024 / 64) + (1024 / 128) + (2048 / 256),
			MAX_ALLOC_SIZE = 5120
		};
	public:
		UMemoryAdminster();
		~UMemoryAdminster();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, _uint& _TableIndex,
			const _uint _Limited, const _uint _AddValue);

	private:
		// 메모리를 빠르게 찾기 위한 풀 테이블이다. 
		ARRAY<UMemoryPool*, MAX_ALLOC_SIZE + 1>	m_PoolTable;
		CONVECTOR<UMemoryPool*>									m_Pools;
	};
	extern class UMemoryAdminster* GAdimister;
	/*
	===========================================================
	MemoryAdminster
	===========================================================
	PoolAllocator
	===========================================================
	*/
	/*
	@ Date: 2024-02-01,  Writer: 박태현
	@ Explain
	- 메모리 풀 객체 할당을 위한 클래스
	*/
	class  UPoolAllocator {
	public:
		static void* Alloc(size_t _size);
		static void Release(void* _ptr);
	};

	/*
	===========================================================
	PoolAllocator
	===========================================================
	Make
	===========================================================
	*/

	namespace Make {

		template<typename Type, typename... Args>
		Type* xnew(Args&&... args)
		{
			Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
			new(memory)Type(std::forward<Args>(args)...); // placement new
			return memory;
		}

		template<typename Type>
		void xdelete(Type* obj)
		{
			obj->~Type();
			UPoolAllocator::Release(obj);
		}

		template<typename Type, typename... Args>
		std::shared_ptr<Type> MakeShared(Args&&... args)
		{
			return std::shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
		}
	}
}