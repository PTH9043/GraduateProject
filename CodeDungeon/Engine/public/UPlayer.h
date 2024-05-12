#pragma once

#include "UCharacter.h"

BEGIN(Engine)
class UAnimationController;
class UCamera;
class UStageManager;
class URegion;
/*
@ Date: 2024-04-30, Writer: 박태현
@ Explain
-  사용자가 직접 조종하는 클래스이다. 
*/
class UPlayer abstract : public UCharacter  {
public:
	struct PLAYERDESC 
	{
		SHPTR<UCamera> spFollowCamera;

		PLAYERDESC() :	spFollowCamera{ nullptr }
		{}
		PLAYERDESC(CSHPTRREF<UCamera> _spFollowCamera) :
			spFollowCamera{ _spFollowCamera }
		{}
	};

	enum {
		PLAYERDESCORDER = 1
	};
public:
	UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UPlayer(const UPlayer& _rhs);
	DESTRUCTOR(UPlayer)
public:
	// UCharacter을(를) 통해 상속됨
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) PURE;
	virtual void Free() PURE;
	virtual HRESULT NativeConstruct() PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) PURE;
protected:
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;

	void FollowCameraMove(const _float3& _vPlayerToDistancePosition, const _double& _TimeElapsed);

protected: /* get set */
	CSHPTRREF<UCamera> GetFollowCamera() const { return m_spFollowCamera; }
private:
	SHPTR<UCamera>		m_spFollowCamera;
};

END