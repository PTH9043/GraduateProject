#include "ClientDefines.h"
#include "CNetworkQueryProcessor.h"
#include "UGameInstance.h"
#include "CWarriorPlayer.h"
#include "CNetworkClientController.h"
#include "UMethod.h"
#include "CMummy.h"
#include "CSarcophagus.h"

CNetworkQueryProcessor::CNetworkQueryProcessor(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController)
	: UNetworkQueryProcessing(_spNetworkBaseController)
{
}

void CNetworkQueryProcessor::MakeActors(const NETWORKRECEIVEINITDATA& _NetworkRecvInitData)
{
#ifdef _ENABLE_PROTOBUFF
	switch (_NetworkRecvInitData.iType)
	{
	case TAG_CHAR::TAG_OTHERPLAYER:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			CWarriorPlayer::CHARACTERDESC CharDesc{ PROTO_RES_FEMAILPLAYERANIMMODEL, PROTO_COMP_NETWORKWARRIORANIMCONTROLLER, true };
			SHPTR<CWarriorPlayer> spWarriorPlayer = std::static_pointer_cast<CWarriorPlayer>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_WARRIORPLAYER, { &CharDesc }));

			spGameInstance->AddCollisionPawnList(spWarriorPlayer);
			GetNetworkBaseController()->AddCreatedNetworkActor(_NetworkRecvInitData.iNetworkID, spWarriorPlayer);
		}
	}
	break;
	case TAG_CHAR::TAG_MUMMY:
	case TAG_CHAR::TAG_SARCOPHAGUS_STANDING:
	case TAG_CHAR::TAG_SARCOPHAGUS_LAYING:
		MakeMonster(_NetworkRecvInitData);
	break;
	}
#endif
}
#ifdef _ENABLE_PROTOBUFF
void CNetworkQueryProcessor::MakeMonster(const NETWORKRECEIVEINITDATA& _NetworkRecvInitData)
{
	CMob::MOBNETWORKDATA mobDataworkData(_NetworkRecvInitData.iCellIndex, _NetworkRecvInitData.vPos, _NetworkRecvInitData.vRotate,
		_NetworkRecvInitData.vScale, _NetworkRecvInitData.iAnimIndex);

	switch (_NetworkRecvInitData.iType)
	{
	case TAG_CHAR::TAG_MUMMY:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			CMummy::CHARACTERDESC CharDesc{ PROTO_RES_MUMMYANIMMODEL, PROTO_COMP_MUMMYANIMCONTROLLER, true };

			SHPTR<CMummy> spMummy = std::static_pointer_cast<CMummy>(spGameInstance->CloneActorAdd(
				PROTO_ACTOR_MUMMY, { &CharDesc, &mobDataworkData }));
			spGameInstance->AddCollisionPawnList(spMummy);
			GetNetworkBaseController()->AddCreatedNetworkActor(_NetworkRecvInitData.iNetworkID, spMummy);
		}
	}
	break;
	case TAG_CHAR::TAG_SARCOPHAGUS_STANDING:
	case TAG_CHAR::TAG_SARCOPHAGUS_LAYING:
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		SHPTR<CSarcophagus> spSarcophagus = nullptr;
		{
			CSarcophagus::CHARACTERDESC CharDesc;
			CharDesc.isNetworkConnected = true;
			CharDesc.wstrAnimControllerProtoData = PROTO_COMP_SARCOPHAGUSANIMCONTROLLER;

			if (_NetworkRecvInitData.iType == TAG_CHAR::TAG_SARCOPHAGUS_LAYING)
			{
				CharDesc.wstrAnimModelProtoData = PROTO_RES_SARCOPHAGUSLYINGANIMMODEL;
				spSarcophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_SARCOPHAGUSLYING, { &CharDesc, &mobDataworkData }));
				spSarcophagus->SetSarcophagusType(CSarcophagus::TYPE_LYING);
			}
			else
			{
				CharDesc.wstrAnimModelProtoData = PROTO_RES_SARCOPHAGUSSTANDINGANIMMODEL;
				spSarcophagus = std::static_pointer_cast<CSarcophagus>(spGameInstance->CloneActorAdd(
					PROTO_ACTOR_SARCOPHAGUSSTANDING, { &CharDesc, &mobDataworkData }));
				spSarcophagus->SetSarcophagusType(CSarcophagus::TYPE_STANDING);
			}
			spGameInstance->AddCollisionPawnList(spSarcophagus);
			GetNetworkBaseController()->AddCreatedNetworkActor(_NetworkRecvInitData.iNetworkID, spSarcophagus);
		}
	}
	break;
	}
}
#endif
void CNetworkQueryProcessor::Free()
{
}
