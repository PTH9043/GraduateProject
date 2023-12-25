#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H


/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: DEBUG_MACRO를 위한 FILE_LINE_FUNCTION등을 정의 
*/
#define DEBUG_MACRO_TO_METHOD \
__FILE__, __LINE__, __FUNCTION__ 
#define LOG_MACRO_TO_METHOD(x) \
typeid(x).name() , __LINE__, __FUNCTION__


/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: namespace의 시작과 끝을 구분하는 영역 지정
*/
#define BEGIN(NAME) namespace NAME{
#define END }

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 조건에 맞으면 리턴하는 메크로
*/
#define RETURN_CHECK(SITUATION, RETURN) if(SITUATION) return RETURN;

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: NULL이면 Message 박스 출력
*/
#ifdef USE_DEBUG

#define ISNULLPTR(T) \
Is_Nullptr_Debug(T, DEBUG_MACRO_TO_METHOD)

#endif 

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: NO_COPY 매크로가 존재하면 복제 불가한 클래스라는 말이다. 
*/
#define NO_COPY(ClassName)							\
private: \
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete; 

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 소멸자에 Free를 호출하게 만드는 매크로
*/
#define DESTRUCTOR(ClassName) virtual ~ClassName() {Free();}

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 헤더파일에 싱글톤에 사용할 함수와 변수를 정의
*/
#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static  SHPTR<ClassName> GetInstance();					\
	static  void DestoryInstance();		\
private:												\
	static SHPTR<ClassName> m_pInstance;

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 헤더파일에서 정의한 함수와 변수를 CPP파일에서 초기화 그리고 정의하여 싱글톤이 되도록한다. 
*/
#define IMPLEMENT_SINGLETON(ClassName)					\
SHPTR<ClassName> ClassName::m_pInstance = nullptr;			\
SHPTR<ClassName> ClassName::GetInstance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = Core::PTH::MakeShared<ClassName>(std::make_shared<ClassName>());		\
	return m_pInstance;									\
}														\
void ClassName::DestoryInstance(){  m_pInstance.reset(); }		

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 싱글톤 클래스의 값을 받아오는 매크로
*/
#define GET_INSTANCE(CLASSNAME) [](){ \
  SHPTR<CLASSNAME> pInstance = CLASSNAME::GetInstance();	\
    return pInstance; \
}();

/*
@ Date: 2023-12-26
@ Writer: 박태현
@ Explain: 강제로 크래쉬를 일으키는 매크로
*/
#define CRASH(Value) Core::DEBUG::Crash(Value);
#define ASSERT_CRASH(COND, Value) Core::DEBUG::AssertCrash(Value, COND);


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H