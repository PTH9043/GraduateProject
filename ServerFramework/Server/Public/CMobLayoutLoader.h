#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H

#include "ACoreBase.h"

BEGIN(Server)

struct MOBDATA {
	Core::_string			strAnimModelName;
	Core::_string			strAnimName;
	Core::_float4x4	mWorldMatrix;

	MOBDATA() : strAnimModelName{ "" }, strAnimName{ "" }, mWorldMatrix{ Core::_float4x4::Identity } {}
};

class CMobLayoutLoader {
public:
	CMobLayoutLoader(const _string& _strPath);
public:
	VECTOR<MOBDATA>& GetMobData() { return m_MobData; }
private:
	VECTOR< MOBDATA>			m_MobData;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H