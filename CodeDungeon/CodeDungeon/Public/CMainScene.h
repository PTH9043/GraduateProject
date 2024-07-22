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
	void TurnGuardsOnRange();
	void UpdateMobsStatus();
public:
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
	void CollisionTick(const _double& _dTimeDelta);

	void DrawStartSceneUI(const _double& _dTimeDelta);
	void CreateStartSceneUI();
	void CreateGameSceneUI();
private:
	// Main
	SHPTR<CMainCamera>			m_spMainCamera;

	SHPTR<CMap>					m_spMap;
	SHPTR<CWarriorPlayer>		m_spWarriorPlayer;

	SHPTR<UFire>	m_stFireOne;
	SHPTR<UFire>	m_stFireTwo;

	_bool			m_bIsFoundPlayer_Minotaur;
	_bool			m_bisFoundPlayer_Harlequinn;
	_bool			m_bisFoundPlayer_Anubis;

	_int			m_iMinotaurHP;
	_int			m_iHarlequinnHP;
	_int			m_iAnubisHP;

	_bool			m_bisMobsAllDead_Interior_Hallway_E;
	_bool			m_bisMobsAllDead_Interior_Room_D;
	_bool			m_bisMobsAllDead_Interior_Room_F;
	_bool			m_bisMobsAllDead_Interior_Room_G;
private:
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
	//==============Boss Frame UI-=================
	//MinoTaur
	SHPTR<CImageUI>			m_spMinotaurFrameUI;
	SHPTR<CHpBarUI>			m_spMinotaurHpBarUI;
	// -------------- Test Font --------------------
	SHPTR<UFont>			m_spTestFont;
};

END

