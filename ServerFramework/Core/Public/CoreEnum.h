#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H


namespace Core
{
	/*
	@ Date: 2023-12-26
	@ Writer: 박태현
	@ Explain:  LOGTYPE의 enum 
	*/
	enum class LOGTYPE : unsigned int
	{
		NONE,
		INFOLOG,						// 일반 정보 로그
		WARNNINGLOG,			// 경고 로그
		ERRORLOG,					// 에러 로그
		DEBUGLOG,			// 디버그용 로그
		CRITICALLOG,			// 치명적인 상황에 남길 로그
		// End 
		LOG_END,
	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H