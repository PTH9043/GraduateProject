#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H

namespace Core
{
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: Cash라인을 16바이트 단위로 맞추기 위해 필요한 변수
	*/
	#define CASH_ALGIN_VALUE 16

#ifdef _WIN64
	using llong = long long;
	using p_size = long long;
#else
	using llong = long;
	using p_size = long;
#endif

	template<class T>
	class URefCounter;

	template<class T>
	class USharedPtr;

	template<class T>
	class UWeakPtr;

	template<class T>
	class UEnableSharedFromThis;

	template<typename Tp, typename = void>
	struct _Can_enable_shared : std::false_type
	{};

	template<typename Tp>
	struct _Can_enable_shared<Tp, std::void_t<typename Tp::_Esft_type>>
		: std::is_convertible<std::remove_cv_t<Tp>*, typename Tp::_Esft_type*>::type
	{};


	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: ReferenceCounter -> 값을 매겨주는 카운터, CAS를 통해 관리
	*/
	template<class T>
	class __declspec(align(CASH_ALGIN_VALUE)) URefCounter {

		template<class T, class U>
		friend USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs);
	public:
		URefCounter(T* _ptr) :
			m_Ptr(_ptr), m_RefStrong(1), m_RefWeak(1) {
		}
		URefCounter(T* _ptr, llong _strong, llong _weak) :
			m_Ptr(_ptr), m_RefStrong{ _strong }, m_RefWeak{ _weak } {
		}

		~URefCounter() { }
	public:

		URefCounter<T>* ComparePointerToStrong()
		{
			while (true)
			{
				llong value = m_RefStrong.load();

				if (value > 0)
				{
					if (true == CAS_VALUE(m_RefStrong, value, value + 1))
					{
						return this;
					}
					else
					{
						continue;
					}
				}

				return nullptr;
			}
			return nullptr;
		}
		llong ReleasePointerToStrong()
		{
			llong value{ 0 };
			// Strong의 값이 0
			while (true)
			{
				value = m_RefStrong.load();
				if (true == CAS_VALUE(m_RefStrong, value, value - 1))
				{
					if (value <= 1)
					{
						Core::MemoryRelease<URefCounter<T>>(this);
					}
					return --value;
				}
			}
			return value;
		}
		URefCounter<T>* ComparePointerToWeak()
		{
			while (true)
			{
				llong value = m_RefWeak.load();
				if (true == CAS_VALUE(m_RefWeak, value, value + 1))
				{
					return this;
				}
			}
			return nullptr;
		}
		llong ReleasePointerToWeak()
		{
			while (true)
			{
				llong value = m_RefWeak.load();
				if (true == CAS_VALUE(m_RefWeak, value, value - 1))
				{
					value -= 1;
					return value;
				}
			}
			return m_RefWeak.load();
		}

		llong GetUseCount() { return m_RefStrong.load() + m_RefWeak.load(); }

		const std::atomic<llong>& GetRefStrongCount() { return m_RefStrong; }
		const std::atomic<llong>& GetRefWeakCount() { return m_RefWeak; }

		T* get() const { return m_Ptr; }

		T* GetShared() const { return m_Ptr; }

		void SetShared(T* _shared) { m_Ptr = _shared; }
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain 
		- _origin의 값이 _old와 같을 때 _weak를 넣고 true 리턴
		*/
		bool CAS_VALUE(std::atomic<llong>& _origin, llong _old, llong _weak)
		{
			return _origin.compare_exchange_strong(_old, _weak);
		}
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain
		- 메모리를 재정렬시킨 후 ptr를 set한다.
		*/
		void SetSharedPtr(T* _ptr)
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			m_Ptr = _ptr;
		}
	private:
		std::atomic<llong>		m_RefStrong;
		std::atomic<llong>		m_RefWeak;
		T*									m_Ptr;
	};

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- std::shared_ptr을 벤치마크 한 스마트 포인터로 멀티쓰레드에서 안전한 스마트 포인터이다.
	*/
	template<class T>
	class   USharedPtr {

		friend class URefCounter<T>;
		friend class UWeakPtr<T>;
		friend  class UEnableSharedFromThis<T>;

		template<class T, class U>
		friend USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs);

		using cas_ptr = std::atomic<llong>;
	public:
		USharedPtr() : m_RefCounter{ nullptr } { }
		USharedPtr(URefCounter<T>* _Ref) : m_RefCounter{ _Ref } {	}
		USharedPtr(nullptr_t) : m_RefCounter{ nullptr } {}
		template<class U>
			requires std::is_base_of_v<T, U>
		USharedPtr(const USharedPtr<U>& _rhs) : m_RefCounter{ nullptr }
		{
			m_RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		}
		USharedPtr(const USharedPtr& _rhs) : m_RefCounter{ nullptr } { Move(_rhs); }

		USharedPtr(USharedPtr&& _rhs) noexcept : m_RefCounter{ _rhs.m_RefCounter } { _rhs.m_RefCounter = nullptr; }

		USharedPtr(const UWeakPtr<T>& _rhs) : m_RefCounter{ _rhs.AddShared() } {  }

		~USharedPtr()
		{
			Release(m_RefCounter);
		}
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain
		- EnableSharedFromThis 클래스를 상속 받는 클래스에 불러올 SharedPtr( 자기 자신의 Weak 포인터를 
		가지게 하기 위함이다.
		*/
		void _Enable_shared_from_this(T* _ptr, std::true_type) {
			_ptr->SetSharedFromThis(*this);
		}
		void _Enable_shared_from_this(T* _ptr, std::false_type) {}
	public:
		void SetEnableShared(T* _ptr, URefCounter<T>* new_ctr)
		{
			m_RefCounter = new_ctr;
			_Enable_shared_from_this(_ptr, std::bool_constant<std::conjunction_v<_Can_enable_shared<T>>>{});
		}

	public:

		USharedPtr& operator =(nullptr_t)
		{
			URefCounter<T>* prevptr{ nullptr };
			while (true)
			{
				prevptr = m_RefCounter;
				if (prevptr == nullptr)
					return *this;

				if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
				{
					Release(prevptr);
					return *this;
				}
			}
			return *this;
		}

		USharedPtr& operator =(const USharedPtr& _rhs)
		{
			return Move(_rhs);
		}

		USharedPtr& operator =(USharedPtr&& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = _rhs.m_RefCounter;
				// 상대방이 null일 때
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						return *this;
					}
					continue;
				}
				// 자기 자신과 같을 때
				if (otherptr == prevptr)
				{
					_rhs.m_RefCounter = nullptr;
					return *this;
				}
				// 상대방과 다른 때
				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					_rhs.m_RefCounter = nullptr;
					return *this;
				}
			}
			return *this;
		}

		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
			requires std::is_base_of_v<T, U>
		USharedPtr& operator =(const USharedPtr<U>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
				// 다른 sharedPtr이 NULL 일때, 현재 값을 Release 한다. 
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}
				// 같은 값일 때, 한쪽 rRelease 
				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}
				// 다른 값일 때, 해당 값과 현재 값을 교환 한 후 본래 값을 Release 한다. 
				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}

		USharedPtr& operator =(const UWeakPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = _rhs.AddShared();
				// 다른 녀석이 null 일때 
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}
				// 값이 같을 떄
				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}
				// 값이 다를 때
				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				// 모두 실패하면 Add 한 값을 Release하고 다시 
				Release(otherptr);
			}

			return *this;
		}

		T* operator->() const { return get(); }
		T& operator*() { return *get(); }

		bool operator ==(nullptr_t _ptr) const { return m_RefCounter == _ptr; }
		bool operator !=(nullptr_t _ptr) const { return m_RefCounter != _ptr; }

		template<class T2>
		bool operator ==(const T2*& _rhs) const { return m_RefCounter == _rhs; }

		template<class T2>
		bool operator !=(const T2*& _rhs) const { return m_RefCounter != _rhs; }

		template<class T2>
		bool operator ==(const USharedPtr<T2>& _rhs) const { return m_RefCounter == _rhs.m_RefCounter; }

		template<class T2>
		bool operator !=(const USharedPtr<T2>& _rhs) const { return m_RefCounter != _rhs.m_RefCounter; }

		T** operator&() const { &m_RefCounter->get(); }

		T* get() const { if (nullptr == m_RefCounter) return nullptr; return m_RefCounter->get(); }

		void reset()
		{
			URefCounter<T>* prevref{ nullptr };
			while (true)
			{
				prevref = m_RefCounter;
				if (prevref == nullptr)
					return;

				if (true == CAS_POINTER(&m_RefCounter, prevref, nullptr))
				{
					Release(prevref);
					return;
				}
			}
		}

		llong use_count()
		{
			return m_RefCounter->GetUseCount();
		}

		operator bool()
		{
			URefCounter<T>* currptr = m_RefCounter;

			if (nullptr != currptr)
				if (currptr->GetUseCount() > 0)
					return true;

			return false;
		}

		URefCounter<T>* AddShared() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = m_RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToStrong();
				curr = m_RefCounter;
				// 현재 가져온 ptr과 curr 값들이 같으면 리턴
				if (getptr == curr)
					return curr;
				// 실패하면 Release 
				if (getptr != nullptr)
					prev->ReleasePointerToStrong();
			}
			return nullptr;
		}
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain
		- Refcounter의 값을 NULL로 초기화
		*/
		void RefValueNull()
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };

			while (true)
			{
				prev = m_RefCounter;
				if (nullptr == prev)
					return;

				if (true == CAS_POINTER(&m_RefCounter, prev, nullptr))
					return;
			}
		}
		URefCounter<T>* AddWeak() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = m_RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToWeak();
				curr = m_RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToWeak();
			}
			return nullptr;
		}

		bool CAS_POINTER(URefCounter<T>** _Counter, URefCounter<T>* _old, URefCounter<T>* _new) const
		{
			llong value1 = reinterpret_cast<llong>(_old);
			llong value2 = reinterpret_cast<llong>(_new);
			return std::atomic_compare_exchange_strong(reinterpret_cast<std::atomic<llong>*>(_Counter), &value1, value2);
		}

		void Release(URefCounter<T>*& _ref)
		{
			if (nullptr == _ref)
				return;

			llong value = _ref->ReleasePointerToStrong();
			if (value <= 0)
			{
				_ref = nullptr;
			}
		}

		USharedPtr<T>& Move(const USharedPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = _rhs.AddShared();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}
		}

	private:
		URefCounter<T>* m_RefCounter;
	};

	template<class T>
	bool operator==(nullptr_t _lhs, const USharedPtr<T>& _rhs) { return _lhs == _rhs.get(); }
	template<class T>
	bool operator!=(nullptr_t _lhs, const USharedPtr<T>& _rhs) { return _lhs != _rhs.get(); }
	template<class T, class U>
	bool operator==(U* _lhs, const USharedPtr<T>& _rhs) { return _lhs == _rhs.get(); }
	template<class T, class U>
	bool operator!=(U* _lhs, const USharedPtr<T>& _rhs) { return _lhs != _rhs.get(); }

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- weak_ptr 대신 멀티쓰레드에서 사용할 수 있는 WeakPtr 정의
	*/
	template<class T>
	class   UWeakPtr {
		friend class URefCounter<T>;
		friend class USharedPtr<T>;
		friend class UEnableSharedFromThis<T>;
	public:
		UWeakPtr() : m_RefCounter{ nullptr } { }
		UWeakPtr(URefCounter<T>* _Ref) : m_RefCounter{ _Ref->ComparePointerToWeak() } {	}
		UWeakPtr(nullptr_t) : m_RefCounter{ nullptr } {}
		template<class U>
		UWeakPtr(const UWeakPtr<U>& _rhs) : m_RefCounter{ _rhs.AddWeak() } {}
		UWeakPtr(const UWeakPtr& _rhs) : m_RefCounter{ _rhs.AddWeak() } {	}
		UWeakPtr(UWeakPtr&& _rhs) : m_RefCounter{ _rhs.m_RefCounter } { _rhs.m_RefCounter = nullptr; }
		UWeakPtr(USharedPtr<T> _sharedPtr) : m_RefCounter{ _sharedPtr.AddWeak() } { }
		~UWeakPtr() { Release(m_RefCounter); }
	public:

		UWeakPtr& operator =(nullptr_t)
		{
			URefCounter<T>* prevptr{ nullptr };
			while (true)
			{
				prevptr = m_RefCounter;
				if (prevptr == nullptr)
					return *this;

				if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
				{
					Release(prevptr);
					return *this;
				}
			}
			return *this;
		}

		UWeakPtr& operator =(const USharedPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = _rhs.AddWeak();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}

		UWeakPtr& operator =(const UWeakPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = m_RefCounter;
				otherptr = _rhs.AddWeak();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&m_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&m_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}

		bool operator ==(nullptr_t _ptr) const { return m_RefCounter == _ptr; }
		bool operator !=(nullptr_t _ptr) const { return m_RefCounter != _ptr; }

		template<class T2>
		bool operator ==(const T2*& _rhs) const { return m_RefCounter == _rhs; }

		template<class T2>
		bool operator !=(const T2*& _rhs) const { return m_RefCounter != _rhs; }

		template<class T2>
		bool operator ==(const USharedPtr<T2>& _rhs) const { return m_RefCounter == _rhs.m_RefCounter; }

		template<class T2>
		bool operator !=(const USharedPtr<T2>& _rhs) const { return m_RefCounter != _rhs.m_RefCounter; }

		USharedPtr<T> lock() const { return USharedPtr<T>(*this); }

		void reset()
		{
			URefCounter<T>* prevref{ nullptr };
			while (true)
			{
				prevref = m_RefCounter;
				if (prevref == nullptr)
					return;

				if (true == CAS_POINTER(&m_RefCounter, prevref, nullptr))
				{
					Release(prevref);
					return;
				}
			}
		}

		llong use_count() const
		{
			return m_RefCounter->GetUseCount();
		}

		operator bool() const
		{
			URefCounter<T>* currptr = m_RefCounter;

			if (nullptr != currptr)
				if (currptr->GetUseCount() > 0)
					return true;

			return false;
		}

		void SetEnableShared(const USharedPtr<T>& _rhs)
		{
			m_RefCounter = _rhs.m_RefCounter->ComparePointerToWeak();
		}
	private:

		URefCounter<T>* AddShared() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = m_RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToStrong();
				curr = m_RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToStrong();
			}
			return nullptr;
		}

		URefCounter<T>* AddWeak() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = m_RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToWeak();
				curr = m_RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToWeak();
			}
			return nullptr;
		}

		bool CAS_POINTER(URefCounter<T>** _Counter, URefCounter<T>* _old, URefCounter<T>* _new) const
		{
			llong value1 = reinterpret_cast<llong>(_old);
			llong value2 = reinterpret_cast<llong>(_new);
			return std::atomic_compare_exchange_strong(reinterpret_cast<std::atomic<llong>*>(_Counter), &value1, value2);
		}

		void Release(URefCounter<T>*& _ptr)
		{
			if (nullptr == _ptr)
				return;

			llong value = _ptr->ReleasePointerToWeak();
			if (value <= 0)
			{
				m_RefCounter = nullptr;
			}
		}
	private:
		mutable URefCounter<T>* m_RefCounter;
	};

	template<typename T>
	class  UEnableSharedFromThis : public  std::enable_shared_from_this<UEnableSharedFromThis<T>> {
		friend  class USharedPtr<T>;
		friend  class URefCounter<T>;
	protected:
		UEnableSharedFromThis()
			: m_Wptr()
		{}

		UEnableSharedFromThis(const UEnableSharedFromThis& other)
			: m_Wptr()
		{}

		UEnableSharedFromThis& operator=(const UEnableSharedFromThis&)
		{
			return (*this);
		}

		~UEnableSharedFromThis() = default;
	public:
		using _Esft_type = UEnableSharedFromThis;

		USharedPtr<T> shared_from_this()
		{
			return (USharedPtr<T>(m_Wptr));
		}

		UWeakPtr<T> weak_from_this()
		{
			return m_Wptr;
		}

		void SetSharedFromThis(const USharedPtr<T>& _ptr) { m_Wptr.SetEnableShared(_ptr); }
	private:
		UWeakPtr<T> m_Wptr;
	};

	template<class T, class ...Args>
	USharedPtr<T> MakeShared(Args&&... _args)
	{
		USharedPtr<T> SharedPtr;
		T* ptr = Core::MemoryAlloc<T>(std::forward<Args>(_args)...);
		URefCounter<T>* pRefCounter = Core::MemoryAlloc<URefCounter<T>>(ptr);
		SharedPtr.SetEnableShared(ptr, pRefCounter);
		return std::move(SharedPtr);
	}

	template<class T, class U>
	USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(static_cast<T*>(_rhs.m_RefCounter->m_Ptr));
		return USharedPtr<T> {RefCounter};
	}

	template<class T, class U>
		requires std::is_base_of_v<T, U> || std::is_base_of_v<U, T>
	USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(TypeCast<U*>(_rhs.m_RefCounter->m_Ptr));
		return USharedPtr<T> {RefCounter};
	}

	template<class T, class U>
	USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(reinterpret_cast<T*>(_rhs.m_RefCounter->m_Ptr));
		return USharedPtr<T> {RefCounter};
	}

	template<class T, class U>
	USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(const_cast<T*>(_rhs.m_RefCounter->m_Ptr));
		return USharedPtr<T> {RefCounter};
	}
}
/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain
- type 재정의
*/
namespace Core {

	template<class T>
	using SHPTR = __declspec(align(CASH_ALGIN_VALUE)) USharedPtr<T>;

	template<class T>
	using CSHPTR = const SHPTR<T>;

	template<class T>
	using CSHPTRREF = const SHPTR<T>&;

	template<class T>
	using WKPTR = __declspec(align(CASH_ALGIN_VALUE))   UWeakPtr<T>;

	template<class T>
	using CWKPTR = const WKPTR<T>;

	template<class T>
	using CWKPTRREF = const WKPTR<T>&;
}

#define CACHE_ALGIN  __declspec(align(CASH_ALGIN_VALUE)) 


namespace std
{
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- HASH opeartor() 연산에 대한 SharedPtr 재정의, concurrent_ 컨테이너에 사용하기 위함
	*/
	template <typename T>
	struct hash<Core::USharedPtr<T>> {
		size_t operator()(const Core::USharedPtr<T>& ptr) const {
			return std::hash<T*>()(ptr.get());
		}
	};
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- Equal 연산에 대한 SharedPtr 재정의, concurrent_ 컨테이너에 사용하기 위함
	*/
	template <typename T>
	struct equal_to<Core::USharedPtr<T>> {
		bool operator()(const Core::USharedPtr<T>& lhs, const Core::USharedPtr<T>& rhs) const {
			return lhs.get() == rhs.get();
		}
	};
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- less 연산에 대한 SharedPtr 재정의, concurrent_ 컨테이너에 사용하기 위함
	*/
	template <typename T>
	struct less<Core::USharedPtr<T>> {
		bool operator()(const Core::USharedPtr<T>& lhs, Core::USharedPtr<T>& rhs) const {
			return lhs.get() < rhs.get();
		}
	};
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H