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
		CONSOL,
		SAVE,
		// End 
		LOG_END,
	};

	enum class SESSIONTYPE : unsigned int {
		OBJECT, PLAYER, MONSTER, TYPE_END
	};

	enum class SERVICETYPE { CLIENT, SERVER, TYPE_END };

	enum {
		MAX_BUFFER_LENGTH = 500, 
		MAX_PROCESSBUF_LENGTH = MAX_BUFFER_LENGTH * 5,
	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H