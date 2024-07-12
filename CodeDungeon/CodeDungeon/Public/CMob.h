#pragma once
#include "UCharacter.h"
#include "UNavigation.h"

BEGIN(Engine)
class UPlayer;
END

BEGIN(Client)
class CMob : public UCharacter {
public:
    struct MOBNETWORKDATA
    {
        _int            iCellIndex;
        _float3      vPos;
        _float3      vRotate;
        _float3      vScale;
        _int            iAnimIndex;

        MOBNETWORKDATA() : iCellIndex(0), vPos{}, vRotate {}, vScale{}, iAnimIndex{ 0 } {}

        MOBNETWORKDATA(_int _iCellIndex, const _float3& _vPos, const _float3& _vRotate, const _float3& _vScale, _int _iAnimIndex)  : 
            iCellIndex(_iCellIndex), vPos{_vPos}, vRotate(_vRotate), vScale{ _vScale }, iAnimIndex{ _iAnimIndex } {}
    };

    enum
    {
        MOBORDER = UCharacter::CHARACTERDESCORDER + 1
    };
public:
    CMob(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
    CMob(const CMob& _rhs);
    DESTRUCTOR(CMob)

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
    void SetFoundTargetState(_bool _isFoundTarget) { this->m_bFoundTarget = _isFoundTarget; }

    const _float& GetDistanceFromPlayer() { return m_fDistancefromNearestPlayer; }
    const _float3& GetTargetPos() { return m_f3TargetPos; }
    void SetMobPlacement(_int _CellIndex);
    void MoveAlongPath(const VECTOR<_float3>& path, size_t& currentPathIndex, const _double& _dTimeDelta);

    _double GetTimeAccumulator() const { return m_dtimeAccumulator; }
    void SetTimeAccumulator(_double _dvalue) { m_dtimeAccumulator = _dvalue; }
    void AddTimeAccumulator(_double _dvalue) { m_dtimeAccumulator += _dvalue; }

    _double GetElapsedTime() const { return m_delapsedTime; }
    void SetElapsedTime(_double _dvalue) { m_delapsedTime = _dvalue; }

    _float GetActivationRange() const { return m_fActivationRange; }
    void SetActivationRange(_float _dvalue) { m_fActivationRange = _dvalue; }
    _float GetDeactivationRange() const { return m_fDeactivationRange; }
    void SetDeactivationRange(_float _dvalue) { m_fDeactivationRange = _dvalue; }


private:
    _float m_fDistancefromNearestPlayer;
    _bool m_bFoundTarget;
    SHPTR<UPlayer> m_spTargetPlayer;
    _float3 m_f3TargetPos;

    _float                          m_fActivationRange;
    _float                          m_fDeactivationRange;


    _double                         m_dtimeAccumulator;
    _double                         m_delapsedTime;
};

END
