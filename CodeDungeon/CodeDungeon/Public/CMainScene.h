#pragma once
#include "UScene.h"
BEGIN(Engine)
class UGuard;
class UFire;
class UFont;
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
class CHpBarUI;

class CMainScene final : public UScene{
public:
	CMainScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CMainScene)
	DESTRUCTOR(CMainScene)
private:
	void TurnLightsOnRange();
	void TurnRoomsOnRange();
	void TurnMobsOnRange();
	void TurnGuardsOnRange();
public:
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
	void CollisionTick(const _double& _dTimeDelta);

	void DrawStartSceneUI(const _double& _dTimeDelta);
	void CreateStartSceneUI();
	void CreateGameSceneUI();
	void CreateKeyInfoUI();
private:
	// Main
	SHPTR<CMainCamera>			m_spMainCamera;

	SHPTR<CMap>							m_spMap;
	SHPTR<CWarriorPlayer>		m_spWarriorPlayer;
	SHPTR<CMummy>				m_spMummy;
	SHPTR<CSarcophagus>			m_spSarcophagus;
	SHPTR<UGuard> m_stGuard;
	SHPTR<UFire>	m_stFireOne;
	SHPTR<UFire>	m_stFireTwo;
private:
	//------------Á¶ÀÛ¹ý Controll Keys UI-----------
	SHPTR<CImageUI>			m_spBOOKPAGEUI;
	SHPTR<CImageUI>			m_spKEYBOARDCONTROLSUI;

	SHPTR<CImageUI>			m_spMOVEUI;
	SHPTR<CImageUI>			m_spMOVETextUI; //wasd - move

	SHPTR<CImageUI>			m_spRUNUI;
	SHPTR<CImageUI>			m_spRUNTextUI; //shift - run

	SHPTR<CImageUI>			m_spJUMPSTANDUPUI;
	SHPTR<CImageUI>			m_spJUMPSTANDUPTextUI;

	SHPTR<CImageUI>			m_spSHORTATTACKUI;
	SHPTR<CImageUI>			m_spSHORTATTACKTextUI;

	SHPTR<CImageUI>			m_spULTIMATEATTACKONEUI;
	SHPTR<CImageUI>			m_spULTIMATEATTACKONETextUI;

	SHPTR<CImageUI>			m_spULTIMATEATTACKTWOUI;
	SHPTR<CImageUI>			m_spULTIMATEATTACKTWOTextUI;

	SHPTR<CImageUI>			m_spDETACTABILITYUI;
	SHPTR<CImageUI>			m_spDETACTABILITYTextUI;

	SHPTR<CImageUI>			m_spROLLUI;
	SHPTR<CImageUI>			m_spROLLTextUI;


	SHPTR<CImageUI>			m_spCOMBOATTACKONEUI;
	SHPTR<CImageUI>			m_spCOMBOATTACKONETextUI;

	SHPTR<CImageUI>			m_spCOMBOATTACKTWOUI;
	SHPTR<CImageUI>			m_spCOMBOATTACKTWOTextUI;

	//------------START SCENE UI-------------------
	SHPTR<CImageUI>			m_spBackgroundUI;
	SHPTR<CImageUI>			m_spMainTitleUI;
	SHPTR<CImageUI>			m_spMainTitleEffectUI;
	SHPTR<CImageUI>			m_spLineEffectUI;
	SHPTR<CImageUI>			m_spBackEffectUI;

	SHPTR<CImageUI>			m_spLoadingBackgroundUI;
	SHPTR<CLoadingUI>			m_spLoadingFillingUI;

	SHPTR<CImageUI>			m_spLoadingTextUI;
	SHPTR<CLoadingUI>			m_spLoadingDotsUI;
	SHPTR<CLoadingUI>			m_spPleaseWaitTextUI;

	SHPTR<CButtonUI>			m_spEnterButtonUI;
	SHPTR<CButtonUI>			m_spExitButtonUI;
	_float m_fStartSceneLoadingTimer = 0;
	_bool m_bStartSceneForUI = false;
	_bool m_bStartGameForUI = false;
	//-------------------------------------------
	//------------GAME SCENE UI-------------------
	SHPTR<CHpBarUI>			m_spHpBarUI;
	SHPTR<CImageUI>			m_spBackPlayerFrameUI;
	SHPTR<CImageUI>			m_spBackDragonPlayerFrameUI;
	SHPTR<CImageUI>			m_spFrontPlayerFrameUI;
	SHPTR<CImageUI>			m_spPlayerNameUI;

	SHPTR<CImageUI>			m_spTABUI;
	SHPTR<CImageUI>			m_spTABTEXTUI;
	//==============Boss Frame UI-=================
	//MinoTaur
	SHPTR<CImageUI>			m_spMinotaurFrameUI;
	SHPTR<CHpBarUI>			m_spMinotaurHpBarUI;
	// -------------- Test Font --------------------
	SHPTR<UFont>			m_spTestFont;
};

END

