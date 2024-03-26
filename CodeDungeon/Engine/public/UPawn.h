#pragma once
#include "UActor.h"

BEGIN(Engine)
class URenderer;
class UShader;
class UCollider;

using COLLIDERCONTAINER = UNORMAP<_wstring, SHPTR<UCollider>>;

class UPawn  abstract : public UActor{
public:
	UPawn(CSHPTRREF<UDevice> _spDevice,  const _wstring & _wstrLayer, const CLONETYPE & _eCloneType, const BACKINGTYPE _eBackingType = BACKINGTYPE::STATIC);
	UPawn(const UPawn& _rhs);
	DESTRUCTOR(UPawn)
public:
	//	CSHPTRREF<URenderer> GetRenderer() const;
		CSHPTRREF<UShader> GetShader() const;

		const 	VECTOR<_uint>& GetRenderingDatas() const { return m_vecRenderingDatas; }
	#ifdef _USE_DEBUGGING
		const std::atomic<_bool>& IsDebugRenderingType() const { return m_isDebugRenderingType; }
	#endif 
		const COLLIDERCONTAINER& GetColliderContainer() const { return m_ColliderContainer; }
	public:
		virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) override PURE;
		virtual void Free() override PURE;
		virtual HRESULT NativeConstruct() override PURE;
		virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override PURE;
		// Hit 
		bool IsHit(CSHPTRREF<UPawn> _spEnemy);

		void AddRenderGroup(const RENDERID _iRenderID);
protected: /* get set*/
	// Get Renderer
	CSHPTRREF<URenderer> GetRenderer() const { return m_spRenderer; }

protected:
		// Tick, LateTick, Render
		virtual void TickActive(const _double& _dTimeDelta) override PURE;
		virtual void LateTickActive(const _double& _dTimeDelta) override PURE;
		virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override PURE;
		// Damaged
		virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;
		// Add Collider
		void AddColliderInContainer(const _wstring& _wstrTag ,const _wstring& _wstrColliderProto, const _float3& _vColliderPos, const _float3& _vColliderScale);
		void AddColliderInContainer(const _wstring& _wstrTag, CSHPTRREF<UCollider> _spCollider);
		// Add Shader
		void AddShader(const _wstring& _wstrProtoTag, const _wstring& _wstrTag = RES_SHADER, const VOIDDATAS& _vecDatas = VOIDDATAS{});
	#ifdef _USE_DEBUGGING
		void AddDebugRenderGroup(const DEBUGRENDERID _iRenderID);
	#endif
	#ifdef _USE_IMGUI
	public:
		virtual void ShowObjectInfo() override;
	#endif
	private:
		SHPTR<URenderer>		m_spRenderer;
		SHPTR<UShader>			m_spShader;

		COLLIDERCONTAINER		m_ColliderContainer;
	#ifdef _USE_DEBUGGING
		VECTOR<_uint>				m_vecRenderingDatas;
		_bool									m_isDebugRenderingType;
	#endif 
};

END