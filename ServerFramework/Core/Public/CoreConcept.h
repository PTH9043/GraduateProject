#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H

#include <iostream>
#include <concepts>

namespace Core
{
	/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 같은 놈인지, 부모 타입인지, operator = 연산 수행 가능한지 
*/
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();

	/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: shared_ptr 전용, 해당 변수가 이동할 수 있는지 체크한다. 
*/
	template <typename T>
	concept SharedPtrConvertible = requires(T t) {
		{ std::shared_ptr<T>(std::move(t)) };
	};

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: Args...에서 펼친 변수들과 생성자의 변수들이 맞는지 확인
	*/
	template<typename T, class ...Args>
	concept ConstructWidthArgsCheck = std::is_constructible_v<T, Args...>;

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: 생성자가 있는지 확인
	*/
	template <typename T, typename DevicePtr, typename GpuCommandPtr>
	concept ConstructArgsMatch = std::is_constructible_v<T, DevicePtr, GpuCommandPtr>;

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: 클래스에 NativeConstruct 함수가 정의되어있는지 확인
	*/
	template <typename T, typename... Args>
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<bool>;
	};

	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);
	template <typename T, typename U>
	concept SharedPtrConvertibleFrom = SharedPtrConvertible<U> && (SameType<T, U> || ParentsChildType<T, U>);
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H