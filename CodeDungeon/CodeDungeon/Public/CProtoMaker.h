#pragma once

BEGIN(Engine)
class UGameInstance;
class UDevice;
class UCommand;
class USkyBox;
END

BEGIN(Client)

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


class CProtoMaker {
public:
	static HRESULT CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance,
		CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand);

	static HRESULT CreateMainSceneProtoData(CSHPTRREF<UGameInstance> _spGameInstance,
		CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand);
};

END

