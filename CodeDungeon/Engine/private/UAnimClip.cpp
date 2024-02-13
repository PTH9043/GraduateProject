#include "EngineDefine.h"
#include "UAnimClip.h"
#include "UAnimation.h"
#include "UAnimModel.h"
#include "UAnimator.h"
#include "AnimEventParents.h"

UAnimClip::UAnimClip(SHPTR<UAnimation> _spAnimation) : 
	m_spAnimation{_spAnimation}
{
}

UAnimClip::UAnimClip(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _Read) :
	m_spAnimation{ nullptr }
{
}

void UAnimClip::Free()
{
}

void UAnimClip::Tick(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta)
{
	ASSERT_CRASH(m_spAnimation);
	for (auto& iter : m_AnimEventContainer)
	{
		for (auto& Event : iter.second)
		{
			Event->EventCheck(_spAnimator, _dTimeDelta, m_spAnimation->GetTimeAcc());
		}
	}
}

void UAnimClip::InsertEvent(ANIMEVENTTYPE _AnimEventType, CSHPTRREF<UAnimEvent> _spAnimEvent)
{
	ASSERT_CRASH(_spAnimEvent);
	const auto& iter = m_AnimEventContainer.find(_AnimEventType);
	if (iter == m_AnimEventContainer.end())
	{
		VECTOR<SHPTR<UAnimEvent>> eventContainer;
		eventContainer.push_back(_spAnimEvent);
		m_AnimEventContainer.emplace(MakePair(_AnimEventType, std::move(eventContainer)));
	}
	else
	{
		iter->second.push_back(_spAnimEvent);
	}
}

void UAnimClip::RemoveEvent(CSHPTRREF<UAnimEvent> _spAnimEvent)
{
	ASSERT_CRASH(_spAnimEvent);
	const auto& iter = m_AnimEventContainer.find(_spAnimEvent->GetAnimEventType());
	RETURN_CHECK(iter == m_AnimEventContainer.end(), ;);
	// Remove 
	iter->second.erase(std::find(iter->second.begin(), iter->second.end(), _spAnimEvent));
}

void UAnimClip::Save(CSHPTRREF<UAnimModel> _spAnimModel, std::ofstream& _Save)
{

}

void UAnimClip::Load(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _Read)
{

}
