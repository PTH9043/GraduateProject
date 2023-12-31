#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H

namespace Core
{
#pragma region FUNCTION 
#ifdef USE_DEBUG
	namespace DEBUG
	{
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain
		- 디버그 메시지를 출력하는 함수, USE_DEBUG에서만 사용
		*/
		static void Debugging_Message(const char* FILE,
			const unsigned int& LINE, const char* FUNC, const _string& _pMessage)
		{
#ifdef USE_DEBUG
			char	szMessage[MAX_PATH] = "";
			_string ClassName = _pMessage;
			_string str = "";
			str += "\nFILE: ";
			str += FILE;
			str += "\n LINE: ";
			str += std::to_string(LINE);
			str += "\n";
			str += FUNC;
			ClassName.append(str);
			MessageBoxA(0, ClassName.c_str(), nullptr, MB_OK);
#endif
		}
		/*
		@ Date: 2023-12-26
		@ Writer: 박태현
		@ Explain
		- 객체를 생성할 때, 에러가 있는 클래스에서만 해당 클래스의 type을 확인하여 메시지를 띄우는 함수, USE_DEBUG에서만 사용
		*/
		template<typename T>
		static void ErrorToCreateClass(T& _rhs) {
			MessageBoxA(0, typeid(T).name(), nullptr, MB_OK);
		}
	}
#endif
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- 단순히 객체를 만드는 함수이다, 
	*/
	template<class T, class... Args>
		requires ConstructWithArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance = Core::MakeShared<T>(std::forward<Args>(args)...);
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
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
			DEBUG::ErrorToCreateClass(pInstance);
#endif
			pInstance.reset();
		}
		return std::move(pInstance);
	}


	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
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
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- NatvieConstruct 함수를 호출 한 후 , 실패하면 nullptr 리턴, 생성자에서 초기화, 
	Natvie함수가 있는지 확인 후, 생성자의 변수와 Args...의 변수가 동일한지 판단.
	*/
	template<class T, class ...Args>
		requires ConstructWithArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateInitConstructor(Args&&... args) {
		static_assert(ConstructWithArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false == (pInstance->NativeConstruct())) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain
	- CreateInitConstructor 동일, 다만 메시지는 출력하지 않음
	*/
	template<class T, class ...Args>
		requires ConstructWithArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T>  CreateInitConstructorNotMsg(Args&&... args) {
		static_assert(ConstructWithArgsCheck<T, Args...>, "생성자의 인자가 잘못되었습니다.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false ==  (pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
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