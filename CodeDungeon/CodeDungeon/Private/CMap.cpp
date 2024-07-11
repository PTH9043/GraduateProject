#include "ClientDefines.h"
#include "UGameInstance.h"
#include "CMap.h"
#include "UCollider.h"
#include "UModel.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UModelMaterial.h"
#include "CRooms.h"
#include "UStageManager.h"
#include "UMapLayout.h"
#include "CTorch.h"
#include "CIronBars.h"
#include "CModelObjects.h"
#include "CMob.h"
#include "CItemChest.h"
#include "CMummy.h"
#include "UAnimModel.h"
#include "UMethod.h"
#include "UNavigation.h"
#include "CWarriorPlayer.h"
#include "CSarcophagus.h"
#include "CMinotaur.h"
#include "CHarlequinn.h"

CMap::CMap(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice),
m_spRoomContainer{nullptr},
m_spMapLayout{ nullptr },
m_spMobsContainer{nullptr},
m_iLightCount{ 0 }
{
}

CMap::CMap(const CMap& _rhs) : UComponent(_rhs),
m_spRoomContainer{ nullptr },
m_spMapLayout{ nullptr },
m_spMobsContainer{ nullptr },
m_iLightCount{0}
{
}

void CMap::Free()
{
	m_spRoomContainer.reset();
	m_spMobsContainer.reset();
}

SHPTR<UCloneObject> CMap::Clone(const VOIDDATAS& _tDatas)
{
	return SHPTR<UCloneObject>();
}

HRESULT CMap::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	m_spRoomContainer = Create<ROOMCONTAINER>();

	m_spMapLayout = CreateConstructorNativeNotMsg<UMapLayout>(spGameInstance->GetDevice());

	m_spStaticObjContainer = Create<STATICOBJCONTAINER>();
	m_spMobsContainer = Create<MOBSCONTAINER>();

	return S_OK;
}

HRESULT CMap::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	return S_OK;
}

void CMap::LoadRooms()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<FILEGROUP> MapFolder = spGameInstance->FindFolder(L"Map");

	for (const FOLDERPAIR& Folder : MapFolder->UnderFileGroupList)
	{
		SHPTR<FILEGROUP> ConvertFolder = Folder.second->FindGroup(L"Convert");
		if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
		{
			for (const FILEPAIR& File : ConvertFolder->FileDataList)
			{
				_wstring FileName = File.second->wstrfileName;

				CRooms::ROOMDESC tDesc;
				tDesc._wsRoomName = FileName;

				SHPTR<CRooms> _Room = std::static_pointer_cast<CRooms>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_ROOM, { &tDesc }));
				m_spRoomContainer->emplace(FileName, _Room);
			}
		}
		else
		{
			return;
		}
	}
}

void CMap::LoadStaticObjects()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spMapLayout->LoadMapObjects();
	OBJCONTAINER _TorchVec;
	OBJCONTAINER _BarsVec;
	for (auto& it : (*m_spMapLayout->GetMapObjectsContainer().get()))
	{
		for (auto& vecit : it.second)
		{
			if(vecit._sModelName == "Torch_FBX.bin")
			{		
				CTorch::TORCHDESC torchDesc;
				torchDesc._Worldm = vecit._mWorldMatrix;
				SHPTR<CTorch> _Torch = std::static_pointer_cast<CTorch>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TORCH, {&torchDesc}));
				_TorchVec.push_back(_Torch);		
			}
			if (vecit._sModelName == "Bars_FBX.bin")
			{
				CIronBars::IRONBARSDESC BarsDesc;
				BarsDesc._Worldm = vecit._mWorldMatrix;
				SHPTR<CIronBars> _IronBars = std::static_pointer_cast<CIronBars>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IRONBARS, { &BarsDesc }));
				_BarsVec.push_back(_IronBars);
				spGameInstance->AddCollisionPawnList(_IronBars);
			}
		}
	}
	m_spStaticObjContainer->emplace("Torch_FBX.bin", _TorchVec);
	m_spStaticObjContainer->emplace("Bars_FBX.bin", _BarsVec);
}

void CMap::LoadMobs(CSHPTRREF<CWarriorPlayer> _spPlayer)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spMapLayout->LoadMapMobs();

	MOBCONTAINER _ChestVec;
	MOBCONTAINER _MummyVec;
	MOBCONTAINER _MinotaurVec;
	MOBCONTAINER _HarlequinnVec;

	for (auto& it : (*m_spMapLayout->GetMapMobsContainer().get()))
	{
		for (auto& vecit : it.second)
		{
			if (vecit._sAnimModelName == "Chest_FBX.bin")
			{
				CItemChest::CHARACTERDESC chestDesc{ PROTO_RES_CHESTANIMMODEL, PROTO_COMP_CHESTANIMCONTROLLER };;
				SHPTR<CItemChest> _Chest = std::static_pointer_cast<CItemChest>(spGameInstance->CloneActorAdd(PROTO_ACTOR_CHEST, { &chestDesc }));
				_Chest->GetTransform()->SetNewWorldMtx(vecit._mWorldMatrix);
				_ChestVec.push_back(_Chest);
				spGameInstance->AddCollisionPawnList(_Chest);
			}
#ifndef _ENABLE_PROTOBUFF
			else if (vecit._sAnimModelName == "Mummy_DEMO_1_FBX.bin")
			{
				CMummy::CHARACTERDESC MummyDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER };

				SHPTR<CMummy> _Mummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_MUMMY, { &MummyDesc }));
				_Mummy->GetTransform()->SetPos(vecit._mWorldMatrix.Get_Pos());
				_Mummy->GetTransform()->SetDirection(vecit._mWorldMatrix.Get_Look());
				_Mummy->GetAnimModel()->SetAnimation(UMethod::ConvertSToW(vecit._sAnimName));
				if (vecit._sAnimName == "staticLaying")
				{
					_Mummy->SetMummyType(CMummy::MUMMYTYPE::TYPE_LYING);			
				}
				else
					_Mummy->SetMummyType(CMummy::MUMMYTYPE::TYPE_STANDING);
				_Mummy->SetTargetPlayer(_spPlayer);
				_Mummy->GetCurrentNavi()->FindCell(_Mummy->GetTransform()->GetPos());
				spGameInstance->AddCollisionPawnList(_Mummy);


				//sarcophagus for mummy
				SHPTR<CSarcophagus> _Sarcophagus;
				if (vecit._sAnimName == "staticLaying")
				{
					CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSLYINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
					_Sarcophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
						PROTO_ACTOR_SARCOPHAGUSLYING, { &SarcDesc }));
					_Sarcophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_LYING);
					_Mummy->GetTransform()->TranslateDir((-_Mummy->GetTransform()->GetLook()), 1, 12);
				}
				else
				{
					CSarcophagus::CHARACTERDESC SarcDesc{ PROTO_RES_SARCOPHAGUSSTANDINGANIMMODEL, PROTO_COMP_SARCOPHAGUSANIMCONTROLLER };
					_Sarcophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
						PROTO_ACTOR_SARCOPHAGUSSTANDING, { &SarcDesc }));
					_Sarcophagus->SetSarcophagusType(CSarcophagus::SARCOTYPE::TYPE_STANDING);
				}
				_Sarcophagus->GetTransform()->SetNewWorldMtx(_Mummy->GetTransform()->GetWorldMatrix());
				_Sarcophagus->GetAnimModel()->SetAnimation(0);
				_Sarcophagus->SetTargetPlayer(_spPlayer);
				_MummyVec.push_back(_Mummy);
				_MummyVec.push_back(_Sarcophagus);
			}
			else if (vecit._sAnimModelName == "minotaur_FBX.bin")
			{
				CMinotaur::CHARACTERDESC MinotaurDesc{ PROTO_RES_MINOTAURANIMMODEL, PROTO_COMP_MINOTAURANIMCONTROLLER };

				SHPTR<CMinotaur> _Minotaur = std::static_pointer_cast<CMinotaur>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_MINOTAUR, { &MinotaurDesc }));
				_Minotaur->GetTransform()->SetPos(vecit._mWorldMatrix.Get_Pos());
				_Minotaur->GetTransform()->SetDirection(vecit._mWorldMatrix.Get_Look());
				_Minotaur->GetAnimModel()->SetAnimation(UMethod::ConvertSToW(vecit._sAnimName));
				_Minotaur->SetTargetPlayer(_spPlayer);
				_Minotaur->GetCurrentNavi()->FindCell(_Minotaur->GetTransform()->GetPos());
				spGameInstance->AddCollisionPawnList(_Minotaur);
				_MinotaurVec.push_back(_Minotaur);
			}
			else if (vecit._sAnimModelName == "Harlequin1_FBX.bin")
			{
				CHarlequinn::CHARACTERDESC HarlequinnDesc{ PROTO_RES_HARLEQUINNANIMMODEL, PROTO_COMP_HARLEQUINNANIMCONTROLLER };

				SHPTR<CHarlequinn> _Harlequinn = std::static_pointer_cast<CHarlequinn>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_HARLEQUINN, { &HarlequinnDesc }));
				_Harlequinn->GetTransform()->SetPos(vecit._mWorldMatrix.Get_Pos());
				_Harlequinn->GetTransform()->SetDirection(vecit._mWorldMatrix.Get_Look());
				_Harlequinn->GetAnimModel()->SetAnimation(UMethod::ConvertSToW(vecit._sAnimName));
				_Harlequinn->SetTargetPlayer(_spPlayer);
				_Harlequinn->GetCurrentNavi()->FindCell(_Harlequinn->GetTransform()->GetPos());
				spGameInstance->AddCollisionPawnList(_Harlequinn);
				_HarlequinnVec.push_back(_Harlequinn);
			}
#endif
		}
	}

	m_spMobsContainer->emplace("Chest_FBX.bin", _ChestVec);
	m_spMobsContainer->emplace("Mummy_DEMO_1_FBX.bin", _MummyVec);
	m_spMobsContainer->emplace("minotaur_FBX.bin", _MinotaurVec);
	m_spMobsContainer->emplace("Harlequin1_FBX.bin", _HarlequinnVec);
}

