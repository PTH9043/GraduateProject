#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H


namespace Core
{
	/*
	@ Date: 2024-01-09,  Writer: 박태현
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

	enum class COLLIDERTYPE {
		AABB, OBB, SPHERE, COLL_END
	};

	enum {
		MAX_BUFFER_LENGTH = 512, 
		MAX_PROCESSBUF_LENGTH = MAX_BUFFER_LENGTH * 5,
		MAX_OCTREENODE_LENGTH = 8, 

	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H