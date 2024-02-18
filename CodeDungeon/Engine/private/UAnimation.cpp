#include "EngineDefine.h"
#include "UAnimation.h"
#include "UChannel.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "AnimEventParents.h"

UAnimation::UAnimation() :
	UBase(),
	m_Channels{},
	m_wstrName{ L"" },
	m_iNumChannels{ 0 },
	m_dTickPerSeconds{ 0 },
	m_dDuration{ 0 },
	m_dTimeAcc{ 0 },
	m_isSupplySituation{ false },
	m_fSupplySituationValue{ 0 },
	m_fTotalAnimationFastValue{1.f},
	m_isFinishAnimation{false}
{
}

UAnimation::UAnimation(const UAnimation& _rhs) :
	UBase(_rhs),
	m_Channels{ _rhs.m_Channels },
	m_wstrName{ _rhs.m_wstrName },
	m_iNumChannels{ _rhs.m_iNumChannels },
	m_dTickPerSeconds{ _rhs.m_dTickPerSeconds },
	m_dDuration{ _rhs.m_dDuration },
	m_dTimeAcc{ 0.0 },
	m_isSupplySituation{ false },
	m_fSupplySituationValue{ 0 },
	m_fTotalAnimationFastValue{ 1.f },
	m_isFinishAnimation{ false }
{
}

void UAnimation::UpdateAnimFastSections(const _float _fTotalAnimFastValue, const VECTOR<ANIMFASTSECTION>& _AnimFastSection)
{
	m_fTotalAnimationFastValue = _fTotalAnimFastValue;
	m_AnimFastSections = _AnimFastSection;
}

SHPTR<UAnimation> UAnimation::Clone(CSHPTRREF<UAnimModel> _spAnimModel)
{
	SHPTR<UAnimation> pAnimation = CloneThis<UAnimation>(*this);

	for (auto& iter : m_Channels)
		pAnimation->m_Channels.push_back(iter->Clone(_spAnimModel));

	return pAnimation;
}

void UAnimation::Free()
{
	m_Channels.clear();
}

HRESULT UAnimation::NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const ANIMDESC& _stAnimDesc)
{
	m_iNumChannels = static_cast<_uint>(_stAnimDesc.Channels.size());
	m_dDuration = _stAnimDesc.stExtraData.dDuration;
	m_dTickPerSeconds = _stAnimDesc.stExtraData.dTickPerSeconds;
	m_wstrName = _stAnimDesc.wstrName;
	m_Channels.reserve(m_iNumChannels);
	for (auto& iter : _stAnimDesc.Channels)
	{
		SHPTR<UChannel> pChannel{ CreateNative<UChannel>(_spAnimModel, iter) };
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void UAnimation::UpdateBoneMatrices(const _double& _dTimeDelta)
{
	_double dValue = m_dTickPerSeconds * _dTimeDelta;
	for (auto& iter : m_AnimFastSections)
	{
		iter.Convert(dValue, m_dTimeAcc);
	}
	m_dTimeAcc += (dValue * m_fTotalAnimationFastValue);

	if (m_dTimeAcc >= m_dDuration)
	{
		m_dTimeAcc = 0.0;
		m_isSupplySituation = false;
		m_isFinishAnimation = true;
	}
	else
	{
		for (auto& iter : m_Channels)
		{
			iter->UpdateTransformMatrix(m_dTimeAcc, this);
		}
		m_isFinishAnimation = false;
	}
}

void UAnimation::UpdateboneMatricesToTimeAcc(const _double& _TimeAcc)
{
	m_dTimeAcc = _TimeAcc;
	for (auto& iter : m_Channels)
	{
		iter->UpdateTransformMatrix(m_dTimeAcc, this);
	}
}

void UAnimation::UpdateNextAnimTransformMatrices(const _double& _dTimeDelta, const _float _fSupplyValue, CSHPTRREF<UAnimation> _spAnimation)
{
	m_fSupplySituationValue += (_float)(_dTimeDelta)*_fSupplyValue;

	if (m_fSupplySituationValue >= MAX_SUPPLY_VALUE)
	{
		m_fSupplySituationValue = 0.001f;
		m_isSupplySituation = false;
	}
	else
	{
		if (_spAnimation->m_Channels.size() > 0)
		{
			m_isSupplySituation = true;
			for (_uint i = 0; i < m_iNumChannels; ++i)
			{
				m_Channels[i]->UpdatSupplyToCurAndNextTransformMatrix(m_dTimeAcc, m_fSupplySituationValue,
					this, _spAnimation->m_Channels[i]);
			}
		}
	}
}

void UAnimation::TickAnimEvent(UAnimModel* _pAnimModel, const _double& _TimeDelta)
{
	for (auto& iter : m_AnimEventContainer)
	{
		for (auto& Event : iter.second)
		{
			Event->EventCheck(_pAnimModel, _TimeDelta, m_dTimeAcc);
		}
	}
}

void UAnimation::ResetData()
{
	m_fSupplySituationValue = 0.001f;
	m_isSupplySituation = false;
	m_isFinishAnimation = false;
	m_dTimeAcc = 0.0;
}

void UAnimation::InsertAnimEvent(ANIMEVENTTYPE _AnimEventType, CSHPTRREF<UAnimEvent> _spAnimEvent)
{
	assert(_AnimEventType <= ANIMEVENTTYPE::ANIMEVENT_END);
	assert(_spAnimEvent);

	const auto& FindIter = m_AnimEventContainer.find(_AnimEventType);
	if (m_AnimEventContainer.end() == FindIter)
	{
		VECTOR<SHPTR<UAnimEvent>> EventVector;
		EventVector.push_back(_spAnimEvent);
		m_AnimEventContainer.emplace(MakePair(_AnimEventType, EventVector));
	}
	else
	{
		FindIter->second.push_back(_spAnimEvent);
	}
}

void UAnimation::RemoveAnimEvent(CSHPTRREF<UAnimEvent> _spAnimEvent)
{
	assert(_spAnimEvent);
	ANIMEVENTTYPE AnimEventType = _spAnimEvent->GetAnimEventType();
	const auto& FindIter = m_AnimEventContainer.find(AnimEventType);
	assert(FindIter != m_AnimEventContainer.end());
	for (auto iter = FindIter->second.begin(); iter != FindIter->second.end(); ++iter)
	{
		if ((*iter) == _spAnimEvent)
		{
			FindIter->second.erase(iter);
			break;
		}
	}
}

/*
====================================================
Save
====================================================
*/
void UAnimation::SaveAnimData(const _wstring& _wstrPath)
{
	std::ofstream Saves{ _wstrPath, std::ios::binary};
	RETURN_CHECK(!Saves, ;);

	_uint iFastSection = static_cast<_uint>(m_AnimFastSections.size());
	Saves.write((_char*)&m_fTotalAnimationFastValue, sizeof(_float));
	// Save Fast Section 
	Saves.write((_char*)&iFastSection, sizeof(_uint));
	for (auto& iter : m_AnimFastSections)
	{
		Saves.write((_char*)&iter, sizeof(ANIMFASTSECTION));
	}
	// Anim Event save 
	{
		// Container Size Save 
		{
			_uint iSize = static_cast<_uint>(m_AnimEventContainer.size());
			Saves.write((_char*)&iSize, sizeof(_uint));
		}
		for (auto& iter : m_AnimEventContainer)
		{
			// Evnet Type Save
			{
				Saves.write((_char*)&iter.first, sizeof(ANIMEVENTTYPE));
			}
			// Event Cnt Save
			{
				_uint EventCnt = static_cast<_uint>(iter.second.size());
				Saves.write((_char*)&EventCnt, sizeof(_uint));
			}
			// Event save
			for (auto& Event : iter.second)
			{
				Event->SaveEvent(Saves);
			}
		}
	}
}

void UAnimation::SaveAnimDataPathIsFolder(const _wstring& _wstrPath)
{
	_wstring str = _wstrPath;
	str.append(L"\\AnimData\\");
	if (0 != _wmkdir(str))
	{
		str.append(m_wstrName);
		str.append(DEFAULT_OUTFOLDEREXTENSION);
		SaveAnimData(str);
	}
}

void UAnimation::LoadAnimData(const _wstring& _wstrPath)
{
	std::ifstream Read{ _wstrPath, std::ios::binary};
	RETURN_CHECK(!Read, ;);

	_uint iFastSection{ 0 };
	Read.read((_char*)&m_fTotalAnimationFastValue, sizeof(_float));
	Read.read((_char*)&iFastSection, sizeof(_uint));
	m_AnimFastSections.resize(iFastSection);
	Read.read((_char*)&m_AnimFastSections[0], sizeof(ANIMFASTSECTION) * iFastSection);
}

void UAnimation::LoadAnimDataPathIsFolder(const _wstring& _wstrPath)
{
	_wstring str = _wstrPath;
	str.append(L"\\AnimData\\");
	str.append(m_wstrName);
	str.append(DEFAULT_OUTFOLDEREXTENSION);

	LoadAnimData(str);
}


/*
====================================================
Save
====================================================
*/