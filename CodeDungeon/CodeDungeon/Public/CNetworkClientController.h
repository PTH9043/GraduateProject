#pragma once
#include "UNetworkBaseController.h"


BEGIN(Client)
class CModelObjects;
class CMob;

using OBJCONTAINER = VECTOR<SHPTR<CModelObjects>>;
using MOBCONTAINER = VECTOR<SHPTR<CMob>>;

using STATICOBJCONTAINER = UNORMAP<_wstring, OBJCONTAINER>;
using MOBSCONTAINER = UNORMAP<_wstring, MOBCONTAINER>;

class CMainCamera;
class CMap;

class CNetworkClientController final : public UNetworkBaseController{
	enum
	{
		MAINCAMERA_ACTORS_ID = 0,
	};
public:
	CNetworkClientController();
	NO_COPY(CNetworkClientController)
	DESTRUCTOR(CNetworkClientController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) override;
	virtual void MakeActorsInit(const VECTOR<SHPTR<UBase>>& _actorContainer) override;
	virtual void MakeActorsTick() override;
protected:
	virtual void NativePacket() override;
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) override;
private:

	void CreateServerMobData(CMap* _pMap);
	void CreateStaticObjData(CMap* _pMap);

	void ConnectSuccessState(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void OtherClientLoginState(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void PlayerAnimState(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void CharDamaged(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void SelfPlayerMoveState(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void MonsterState(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void MonsterFind(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void StaticObjFind(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void HarlequinThrowing(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void SavePointEnabe(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void PlayerGetup(_char* _pPacket, const PACKETHEAD& _PacketHead);
private:
	virtual void Free() override;
private:
};

END