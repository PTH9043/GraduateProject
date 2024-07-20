#pragma once
#include "UScene.h"
BEGIN(Engine)
class UGuard;
class UFire;
END
BEGIN(Client)

class CMainCamera;
class CMap;
class CWarriorPlayer;
class CMummy;
class CSarcophagus;
class CImageUI;
class CButtonUI;
class CLoadingUI;


class CMainScene final : public UScene{
public:
	CMainScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CMainScene)
	DESTRUCTOR(CMainScene)
private:
	//기능 함수들 일시
	void TurnLightsOnRange();
	void TurnRoomsOnRange();
	void TurnMobsOnRange();
	void TurnGuardsOnRange();
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
	void CollisionTick(const _double& _dTimeDelta);

	void DrawStartSceneUI(const _double& _dTimeDelta);
private:
	// Main
	SHPTR<CMainCamera>			m_spMainCamera;

	SHPTR<CMap>							m_spMap;
	SHPTR<CWarriorPlayer>		m_spWarriorPlayer;
	SHPTR<CMummy>				m_spMummy;
	SHPTR<CSarcophagus>			m_spSarcophagus;
	SHPTR<UGuard> m_stGuard;
	SHPTR<UFire> m_stFireOne;
	SHPTR<UFire> m_stFireTwo;
private:
	//------------START SCENE UI-------------------
	SHPTR<CImageUI>			m_spBackgroundUI;
	SHPTR<CImageUI>			m_spMainTitleUI;
	SHPTR<CImageUI>			m_spBackEffectUI;

	SHPTR<CImageUI>			m_spLoadingBackgroundUI;
	SHPTR<CLoadingUI>			m_spLoadingFillingUI;

	SHPTR<CButtonUI>			m_spButtonUI;
	_float m_fStartSceneLoadingTimer = 0;
	_bool m_bStartScene = false;
	//-------------------------------------------

};

END

