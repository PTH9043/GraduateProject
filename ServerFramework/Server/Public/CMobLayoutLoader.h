#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H

#include "ACoreBase.h"

BEGIN(Server)

class CMobLayoutLoader {
public:
	CMobLayoutLoader(const _string& _strPath, const _string& _layoutName);
public:
	VECTOR<MOBDATA>& GetMobData() { return m_MobData; }
private:
	VECTOR< MOBDATA>			m_MobData;
};

class CStaticObjectLayoutLoader {
public:
	CStaticObjectLayoutLoader(const _string& _strPath, const _string& _layoutName);
public:
	VECTOR<MOBDATA>& GetMobData() { return m_MobData; }
private:
	VECTOR< MOBDATA>			m_MobData;
};


class CMobServerLayoutLoader {
public:
	CMobServerLayoutLoader(const _string& _strPath);
public:
	VECTOR<MOBSERVERDATA>& GetMobData() { return m_MobData; }
private:
	VECTOR< MOBSERVERDATA>			m_MobData;
};

class CStaticObjServerLayoutLoader {
public:
	CStaticObjServerLayoutLoader(const _string& _strPath);
public:
	VECTOR<STATICOBJSERVERDATA>& GetStaticObjData() { return m_StaticObjData; }
private:
	VECTOR< STATICOBJSERVERDATA>			m_StaticObjData;
};



END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMOBLAYOUTLOADER_H