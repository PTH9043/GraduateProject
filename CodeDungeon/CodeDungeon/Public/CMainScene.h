#pragma once
#include "UScene.h"
BEGIN(Engine)
END
BEGIN(Client)

class CMainCamera;
class CMap;
class CWarriorPlayer;
class CMummy;
class CSarcophagus;


class CMainScene final : public UScene{
public:
	CMainScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CMainScene)
	DESTRUCTOR(CMainScene)
private:
	//기능 함수들 일시
	void TurnLightsOnRange();
	void TurnRoomsOnRange();
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
private:
	// Main
	SHPTR<CMainCamera>			m_spMainCamera;

	SHPTR<CMap>							m_spMap;
	SHPTR<CWarriorPlayer>		m_spWarriorPlayer;
	SHPTR<CMummy>				m_spMummy;
	SHPTR<CSarcophagus>			m_spSarcophagus;

};

END

