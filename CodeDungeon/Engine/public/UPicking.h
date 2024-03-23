#pragma once

#include "UBase.h"

BEGIN(Engine)
class UActor;
class UTransform;
class UVIBuffer;
class UGameInstance;
class UGrid;
class UCollider;

struct PICKINGDESC {
	PICKINGDESC() = default;
	PICKINGDESC(CSHPTRREF<UActor> _spActor, const _float3& _vPickPos, const _float _fDist, _bool _bSuccess)
		: spActor(_spActor), vPickPos(_vPickPos), fDist(_fDist), bPickingSuccess(_bSuccess)
	{}
	SHPTR<UActor>	spActor{ nullptr };
	_float3			vPickPos{ 0, 0, 0 };
	_float			fDist{ 0 };
	_bool			bPickingSuccess{ false };
};

struct WAITCHECKACTOR {
	SHPTR<UActor>			spActor{ nullptr };
	SHPTR<UVIBuffer>		spVIBuffer{ nullptr };

	_bool operator == (const WAITCHECKACTOR& _stActor) const
	{
		if (spActor == _stActor.spActor && spVIBuffer == _stActor.spVIBuffer)
			return true;

		return false;
	}
};

struct MAINGRID {
	SHPTR<UGrid> spGrid{ nullptr };
	_bool operator == (const MAINGRID& _stGrid) const
	{
		if (spGrid == _stGrid.spGrid)
			return true;

		return false;
	}
};

END

namespace std
{
	// Atomic 함수 재정의
	template<>
	struct hash<Engine::WAITCHECKACTOR> {
		size_t operator()(WAITCHECKACTOR ptr) const {
			return std::hash<WAITCHECKACTOR*>()(&ptr);
		}
	};
	template<>
	struct equal_to<Engine::WAITCHECKACTOR> {
		bool operator()(const Engine::WAITCHECKACTOR& lhs, const Engine::WAITCHECKACTOR& rhs) const {
			return lhs == rhs;
		}
	};
}

BEGIN(Engine)

class UPicking : public UBase {
public:
	UPicking();
	NO_COPY(UPicking)
	DESTRUCTOR(UPicking)
public:
	virtual void Free() override;
	HRESULT ReadyPickingDesc(CSHPTRREF<GRAPHICDESC> _spGraphicDesc);
	void CastRayInWorldSpace(UGameInstance* _pGameInstance);
	void AddPickingObject(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer);
	void AddPickingGrid(const MAINGRID& _stGrid);
	SHPTR<UActor> GetPickingActor();
	const PICKINGDESC& GetPickDesc();
	_bool PickingMesh(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer,
		_float* _pDist, _float3* _pOut);
	_bool PickingOnGrid(CSHPTRREF<UGrid> _spGrid, _float* _pDist, _float3* _pOut);
private:
	_bool IsPickingCheck(const _float3& _vLocalRay, const _float3& _vDirRay, const _float3& _vPos1,
		const _float3& _vPos2, const _float3& _vPos3, const _float4x4& _mWorldMatrix, _float* _pDist, _float3* _pOut);
	// Add Picking Desc
	void AddPickingObject(const PICKINGDESC& _stDesc);
private:
	using PICKINGLIST = std::list<PICKINGDESC>;
	using WAITCHECKACTORLIST = UNORSET<WAITCHECKACTOR>;
	// // Dats
	_float2					m_vWindowSize;
	RECT					m_stClientRect;
	_float3					m_vRayPos;
	_float3					m_vRayDir;
	PICKINGLIST				m_lsPickingList;
	PICKINGDESC				m_stPickingDesc;
	WAITCHECKACTORLIST		m_WaitCheckActorList;
	MAINGRID				m_spMainGrid;
	_bool					m_isMouseInScreen;
};

END

