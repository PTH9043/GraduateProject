#pragma once
#include "UPlayer.h"
#include "UCell.h"

BEGIN(Engine)
class UGameInstance;
class UParticle;
class UParticleSystem;
class UTrail;
class UBlood;
class UDust;
END

BEGIN(Client)
class CSword;
/*
@ Date: 2024-04-30, Writer: 
@ Explain
*/
class CWarriorPlayer final : public UPlayer {
public:
	CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CWarriorPlayer(const CWarriorPlayer& _rhs);
	DESTRUCTOR(CWarriorPlayer)
public:
	CLONE_MACRO(CWarriorPlayer, "CWarriorPlayer::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	virtual void ReceiveNetworkProcessData(const UProcessedData& _ProcessData);
public: /* Get Set */
	SHPTR<UParticle>& GetParticle() { return m_spFootPrintParticle; }

	void IfAttack(_bool is) { isAttack = is; }

	const _bool& GetOBJCollisionState() const { return m_bisCollisionWithObj; }
	void SetOBJCollisionState(_bool _newState) { m_bisCollisionWithObj = _newState; }

	const _bool& GetKickedState() const { return m_bisKicked; }
	void SetKickedState(_bool _newState) { m_bisKicked = _newState; }

	const _bool& GetRiseState() const { return m_bisRise; }
	void SetRiseState(_bool _newState) { m_bisRise = _newState; }

	const _bool& GetIfOpenChest() const { return IfOpenChestForHeal; }
	void SetIfOpenChest(_bool _newState) { IfOpenChestForHeal = _newState; }

	const _double& GetWarriorKickedTimeElapsed() { return m_dKickedElapsed; }
	void SetWarriorKickedTimeElapsed(_double _newTime) { m_dKickedElapsed = _newTime; }

	void SetCanInteractChestState(const _bool& _newState) { m_bCanInteractChest = _newState; }
	_bool GetCanInteractChestState() { return m_bCanInteractChest; }
	void SetCanInteractBarState(const _bool& _newState) { m_bCanInteractBar = _newState; }
	_bool GetCanInteractBarState() { return m_bCanInteractBar; }
	void SetCanInteractStatueState(const _bool& _newState) { m_bCanInteractStatue = _newState; }
	_bool GetCanInteractStatueState() { return m_bCanInteractStatue; }

	_float GetInteractionElapsedTime() { return m_fInteractionTimeElapsed; }

	CSHPTRREF<UCell> GetSpawnPointCell() { return m_spSpawnPoint; }
	const _float3& GetSpawnPointPos() { return m_f3SpawnPoint; }
	void SetSpawnPoint(SHPTR<UCell> _newSpawnPoint) { m_spSpawnPoint = _newSpawnPoint; }
	void SetSpawnPoint(_float3 _newSpawnPoint) { m_f3SpawnPoint = _newSpawnPoint; }
	CSHPTRREF<UCell> GetSpawnPointCamera() { return m_spSpawnPointForCamera; }
	void SetSpawnPointCamera(SHPTR<UCell> _newSpawnPoint) { m_spSpawnPointForCamera = _newSpawnPoint; }
	

protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual void NetworkTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
#ifdef _ENABLE_PROTOBUFF
	void SendMoveData();
	void SendCollisionData(UPawn* _pPawn);
#endif
private:
	void TranslateStateMoveAndRunF(CSHPTRREF<UGameInstance> _spGameInstance, const _double& _dTimeDelta, const _float _fSpeed);
public:
	_float GetUltAttackOneCoolTime();
	_float GetUltAttackTwoCoolTime();
	_float GetShortAttackCoolTime() ;
	_bool GetBlindEffectBool();
	_bool GetDieEffectBool();
private:
	SHPTR<UParticle>										m_spFootPrintParticle;
	SHPTR<UParticle>										m_spHealParticle;
	_float HealTimer = 0;
	_bool HealTrigger = false;
	_bool IfOpenChestForHeal = false;
	PARTICLEPARAM* m_stParticleParam;
	ComputeParticleType* m_stParticleType;

	SHPTR<CSword>												m_spSword;
	SHPTR<UTrail>												m_spTrail;
	SHPTR<UBlood>												m_spBlood;
	SHPTR<UDust>												m_spDust;
	_bool																isAttack;

	_bool m_bisCollisionWithObj;
	_bool m_bisKicked;
	_bool m_bisRise;

	_double											m_dKickedElapsed;
	_bool											m_bCanInteractChest;
	_bool											m_bCanInteractBar;
	_bool											m_bCanInteractStatue;
	_bool											m_bCanInteractGuard;

	_float			m_fInteractionTimeElapsed=0;
	SHPTR<UCell>			m_spSpawnPoint;
	SHPTR<UCell>			m_spSpawnPointForCamera;
	_float3					m_f3SpawnPoint;
};
END

