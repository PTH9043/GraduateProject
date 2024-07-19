#include "UUserInterface.h"

BEGIN(Engine)
class UTexGroup;
class UShaderConstantBuffer;
END

BEGIN(Client)

class CButtonUI final : public UUserInterface {
	struct BUTTONDESC
	{
		_float3 _vButtonColor;
		_float _durationTime;
	};
public:
	CButtonUI(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CButtonUI(const CButtonUI& _rhs);
	DESTRUCTOR(CButtonUI)
public:
	CLONE_MACRO(CButtonUI, "CButtonUI::Clone To Failed")
		virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Ready 
	virtual HRESULT ReadyRectTransform(const VOIDDATAS& _vecDatas) override;

	// UUserInterface을(를) 통해 상속됨
	HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass) override;
	void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;

public:
	void SetButtonColor(_float3 _col) {
		_buttonDesc._vButtonColor = _col;
	}
	void SetIfPicked(_bool _picked) {
		isPicked = _picked;
	}

private:
	// UI Texture Group 
	SHPTR<UTexGroup>			m_spUITextureGroup;
	SHPTR< UShaderConstantBuffer>	m_spButtonUIBuffer;
	BUTTONDESC _buttonDesc;
	_bool isPicked = false;
};


END
