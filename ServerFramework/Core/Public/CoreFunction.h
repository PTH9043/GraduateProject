#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H

namespace Core
{
#pragma region FUNCTION 

	template<class T, class Value >
	requires (Number<T> && Number<Value>)
	T CheckOverValue(const T& _temp, const Value& _Value)
	{
		if (_temp >= _Value)
		{
			return static_cast<T>(_Value);
		}
		return _temp;
	}

	template<class T>
	void MemoryInitialization(T* _pData, const _int _bufferSize)
	{
		std::memset(_pData, 0, _bufferSize);
	}

	template<class T>
	requires std::is_integral_v<T>
	void ThreadRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(_data));
	}

	/*
	@ Date: 2023-01-02,  Writer: 박태현
	@ Explain
	- 멀티쓰레드에서 안전하게 값을 바꾸기 위한 함수
	*/
	template<class T, class Value>
	bool CAS(T* _pData, Value _expected, const Value& _value) {
		return std::atomic_compare_exchange_strong(
			reinterpret_cast<volatile ATOMIC<T>*>(_pData), &_expected, _value);
	}
	
	/*
	@ Date: 2023-01-02,  Writer: 박태현
	@ Explain
	- 멀티쓰레드에서 안전하게 값을 바꾸기 위한 함수
	*/
	template<class T, class Value>
	bool CAS(REF_IN std::atomic<T>& _Data, Value _expected, const Value& _value) {
		return _Data.compare_exchange_strong(_expected, _value);
	}

	template<class T1, class T2>
	std::pair<T1, T2> MakePair(T1& _t1, T2& _t2) { return std::move(std::pair<T1, T2>(_t1, _t2)); }

	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- 단순히 객체를 만드는 함수이다, 
	*/
	template<class T, class... Args>
		requires ConstructArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance = Core::MakeShared<T>(std::forward<Args>(args)...);
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- NatvieConstruct 함수를 호출 한 후 , 실패하면 nullptr 리턴, 함수에서 초기화
	*/
	template<class T, class ...Args>
		requires	CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNative(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>() };
		if (false ==  (pInstance->NativeConstruct(args...))) {
#ifdef USE_DEBUG
			CRASH("SHPTR<T>::CreateInitNative");
#endif
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- CreateInitNative 동일, 다만 메시지는 출력하지 않음
	*/
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNativeNotMsg(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>() };
		if (false == (pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- NatvieConstruct 함수를 호출 한 후 , 실패하면 nullptr 리턴, 생성자에서 초기화, 
	Natvie함수가 있는지 확인 후, 생성자의 변수와 Args...의 변수가 동일한지 판단.
	*/
	template<class T, class ...Args>
		requires ConstructArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateInitConstructor(Args&&... args) {
		static_assert(ConstructArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false == (pInstance->NativeConstruct())) {
#ifdef USE_DEBUG
			CRASH("SHPTR<T>::CreateInitConstructor");
#endif
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- CreateInitConstructor 동일, 다만 메시지는 출력하지 않음
	*/
	template<class T, class ...Args>
		requires ConstructArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T>  CreateInitConstructorNotMsg(Args&&... args) {
		static_assert(ConstructArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false ==  (pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain
	- 자기 자신을 클론하는 함수이다. 
	*/
	template<class T>
	static SHPTR<T> CloneThis(const T& _rhs)
	{
		SHPTR<T> pInstance{ Core::MakeShared<T>(_rhs) };
		return std::move(pInstance);
	}

#pragma endregion FUNCTION
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H