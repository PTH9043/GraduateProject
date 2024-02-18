#pragma once

#include "UPawn.h"

BEGIN(Engine)
class UAnimModel;
class UAnimation;
END

BEGIN(Tool)

using ANIMMATIONCLIPS = UNORMAP<_string, _int>;

// Animation ColumID 
enum ANIM_TABLE {
	AnimColumnID_StartSpot,
	AnimColumnID_EndSpot,
	AnimColumnID_FastValue,
};

/*
@ Date: 2024-02-06, Writer: 박태현
@ Explain
- Animation 데이터들을 Modify하고 Animation과 Animation을 이어주는 Tool을 만들기 위해 필요한 객체
*/
class TAnimControlModel final : public UPawn {
public:
	TAnimControlModel(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TAnimControlModel(const TAnimControlModel& _rhs);
	DESTRUCTOR(TAnimControlModel)
public: /* get set*/
	CSHPTRREF<UAnimModel> GetAnimModel() const { return m_spModel; }
	const ANIMMATIONCLIPS& GetAnimationClips() const { return m_AnimationClips; }
public:
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(TAnimControlModel, "TAnimControlModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	void ReleaseShowModel();
	void SetShowModel(CSHPTRREF<UAnimModel> _spModel, CSHPTRREF<FILEGROUP> _spFileFolder);
	void ShowAnimModify();
protected:
	void SelectAnimation();
	void ModifyAnimation();
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UAnimModel>					m_spModel;
	SHPTR<FILEGROUP>						m_spModelFolder;
	SHPTR<UAnimation>						m_spCurAnimation;
	ANIMMATIONCLIPS							m_AnimationClips;
	ANIMMATIONCLIPS							m_FindAnimClips;

	VECTOR<ANIMFASTSECTION>		m_AnimFastSections;
	_bool													m_isAnimationStop;
	_float													m_fAnimTimeAcc;
	_float													m_fTotalAnimFastvalue;
	_bool													m_isAnimEventActive;
};

END