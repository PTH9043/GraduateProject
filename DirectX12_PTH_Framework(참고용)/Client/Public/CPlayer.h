#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H

#include "UCharacter.h"

BEGIN(Engine)
class UVIBufferSphere;
class UTexGroup;
class UTerrain;
class UShaderConstantBuffer;
class UCollider;
END

BEGIN(Client)
class CMainCamera;
class CPlayerGun;
class CMonster;

class CPlayer final : public UCharacter {
public:
	CPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CPlayer(const CPlayer& _rhs);
	DESTRUCTOR(CPlayer)
public:
	CLONE_MACRO(CPlayer, "Player::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override ;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override ;
	void CollisionOthers(CSHPTRREF< CMonster> _spMonster);

	CSHPTRREF<UCollider> GetCollider() const { return m_spCollider; }
	CSHPTRREF<UCollider> GetFirstCheckCollider() const { return m_spFirstCheckCollider; }
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override ;
	virtual void LateTickActive(const _double& _dTimeDelta) override ;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override ;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) ;
private:
	// Cube TexGroup
	SHPTR<UVIBufferSphere>					m_spVIBufferSphere;
	SHPTR<UTexGroup>							m_spTexGroup;
	SHPTR<UTerrain>								m_spTerrain;
	SHPTR<CMainCamera>						m_spMainCamera;

	_float3													m_vPrevPos;

	SHPTR< UCollider>								m_spCollider;
	SHPTR<UCollider>								m_spFirstCheckCollider;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H