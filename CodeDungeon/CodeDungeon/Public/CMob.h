#pragma once
#include "UCharacter.h"

BEGIN(Engine)
class UPlayer;
END


BEGIN(Client)
class CMob : public UCharacter {
public:
	CMob(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CMob(const CMob& _rhs);


	DESTRUCTOR(CMob)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CMob, "CMob::CloneToFailed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;


protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
	void SearchForPlayers();
	void CalculateDistanceBetweenPlayers(const _float3& _CurrentPlayerPos, const _float3& _CurrentMobPos);

public:
	CSHPTRREF<UPlayer> GetTargetPlayer() { return m_spTargetPlayer; }
	void SetTargetPlayer(CSHPTRREF<UPlayer> _targetPlayer) { m_spTargetPlayer = _targetPlayer; }
	const _bool& GetFoundTargetState() { return m_bFoundTarget; }
	const _float& GetDistanceFromPlayer() { return m_fDistancefromNearestPlayer; }
	const _float3& GetTargetPos() { return m_f3TargetPos; }
	void SetMobPlacement(_int _CellIndex);
	void MoveAlongPath(const VECTOR<_float3>& path, size_t& currentPathIndex, const _double& _dTimeDelta);
	
private:
	_float								m_fDistancefromNearestPlayer;
	_bool								m_bFoundTarget;
	SHPTR<UPlayer>			m_spTargetPlayer;
	_float3							 m_f3TargetPos;
};

END