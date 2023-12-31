#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H

#include "UBase.h"
#include "aws/gamelift/model/GameSession.h"

BEGIN(Core)
/*
@ Date: 2023-12-31
@ Writer: 박태현
@ Explain
- Aws를 이용하기 위한 Context 객체이다. 
해당 객체를 부모로해서 Server 프로젝트에서 자식 클래스를 재정의하여 사용한다. 
*/
class CACHE_ALGIN_CORE_DLL UAwsContext abstract : public UBase {

};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAWSCONTEXT_H