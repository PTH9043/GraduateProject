#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H

#include <iostream>
#include <concepts>

namespace Core
{
	/*
	@ Date: 2024-01-05, Writer: 박태현
	@ Explain: 해당하는 Template가 숫자인지 확인
	*/
	template <typename T>
	concept Number = std::integral<T> || std::floating_point<T>;

	/*
@ Date: 2023-12-26, Writer: 박태현
@ Explain: 같은 놈인지, 부모 타입인지, operator = 연산 수행 가능한지 
*/
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();


	template <typename Base, typename Derived>
	concept BASE_CHECK = std::is_base_of_v<Base, Derived>;

	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain: Args...에서 펼친 변수들과 생성자의 변수들이 맞는지 확인
	*/
	template<typename T, class ...Args>
	concept ConstructArgsCheck = std::is_constructible_v<T, Args...>;

	/*
	@ Date: 2023-12-26, Writer: 박태현
	@ Explain: 클래스에 NativeConstruct 함수가 정의되어있는지 확인
	*/
	template <typename T, typename... Args>
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<bool>;
	};


	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: SmartPointer인지 확인하는 Concept 
	*/
	template <typename T>
	concept SmartPointer = requires(T t) {
		{ t.get() } -> std::same_as<typename T::element_type*>;
		{ t.reset() } -> std::same_as<void>;
	};

	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain: Pointer의 배열인지 확인하는 conecept
	*/
	template <typename T>
	concept PointerArray = requires(T t) {
		{ *t } -> std::same_as<std::remove_extent_t<T>>;  
	};

	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H