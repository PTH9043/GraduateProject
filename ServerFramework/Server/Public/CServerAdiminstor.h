#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H

#include "UServerService.h"

BEGIN(Server)

/*
@ Date: 2023-01-02, Writer: 박태현
@ Explain
- Server를 관리하는 관리자 클래스이다. 플레이어 세션들을 총괄
*/
class CACHE_ALGIN CServerAdiminstor final : public Core::UServerService{
public:
	CServerAdiminstor(OBJCON_CONSTRUCTOR);
	NO_COPY(CServerAdiminstor)
	DESTRUCTOR(CServerAdiminstor)
public:
	virtual bool NativeConstruct() override;
protected:
	// UServerService을(를) 통해 상속됨
	virtual void Connect() override;
private:
	void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
